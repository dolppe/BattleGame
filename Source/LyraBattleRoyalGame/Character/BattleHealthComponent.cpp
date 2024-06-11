#include "BattleHealthComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "GameplayMessageSubsystem.h"
#include "NativeGameplayTags.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"
#include "LyraBattleRoyalGame/BattleGameplayTags.h"
#include "LyraBattleRoyalGame/BattleLogChannels.h"
#include "LyraBattleRoyalGame/AbilitySystem/BattleAbilitySystemComponent.h"
#include "LyraBattleRoyalGame/AbilitySystem/Attributes/BattleHealthSet.h"
#include "LyraBattleRoyalGame/Messages/BattleVerbMessage.h"
#include "LyraBattleRoyalGame/Messages/BattleVerbMessageHelpers.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleHealthComponent)

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Battle_Elimination_Message, "Battle.Elimination.Message");

UBattleHealthComponent::UBattleHealthComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
		// 이벤트기반으로 Character와 PlayerState의 HealthSet의 브릿지 역할만 해줌.
    	// 따라서 틱이 필요가 없음.
    	PrimaryComponentTick.bStartWithTickEnabled = false;
    	PrimaryComponentTick.bCanEverTick = false;
    
    	AbilitySystemComponent = nullptr;
    	HealthSet = nullptr;
}

UBattleHealthComponent* UBattleHealthComponent::FindHealthComponent(const AActor* Actor)
{
	if (!Actor)
	{
		return nullptr;
	}

	UBattleHealthComponent* HealthComponent = Actor->FindComponentByClass<UBattleHealthComponent>();
	return HealthComponent;
}

float UBattleHealthComponent::GetHealth() const
{
	return (HealthSet ? HealthSet->GetHealth() : 0.0f);
}

float UBattleHealthComponent::GetMaxHealth() const
{
	return (HealthSet ? HealthSet->GetMaxHealth() : 0.0f);
}

float UBattleHealthComponent::GetHealthNormalized() const
{
	if (HealthSet)
	{
		const float Health = HealthSet->GetHealth();
		const float MaxHealth = HealthSet->GetMaxHealth();
		return ((MaxHealth > 0.0f) ? (Health/MaxHealth) : 0.0f);
	}
	return 0.0f;
}

void UBattleHealthComponent::InitializeWithAbilitySystem(UBattleAbilitySystemComponent* InASC)
{
	AActor* Owner = GetOwner();
	check(Owner);

	if (AbilitySystemComponent)
	{
		UE_LOG(LogBattle, Error, TEXT("BattleHealthComponent : HealthComponent for Owner has already been initialized with ASC"));
		return;
	}

	AbilitySystemComponent = InASC;
	
	if (!AbilitySystemComponent)
	{
		UE_LOG(LogBattle, Error, TEXT("BattleHealthComponent : Cannot initialize health Component becuase ASC Is null"));
		return;
	}

	HealthSet = AbilitySystemComponent->GetSet<UBattleHealthSet>();
	if (!HealthSet)
	{
		UE_LOG(LogBattle, Error, TEXT("BattleHealthComponent : Cannot initialize health Component becuase HealthSet Is null"));
		return;
	}

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(UBattleHealthSet::GetHealthAttribute()).AddUObject(this, &ThisClass::HandleHealthChanged);
	HealthSet->OnOutOfHealth.AddUObject(this, &ThisClass::HandleOutOfHealth);
	
	OnHealthChanged.Broadcast(this, HealthSet->GetHealth(), HealthSet->GetHealth(), nullptr);
	
}

void UBattleHealthComponent::UnInitializeWithAbilitySystem()
{
	AbilitySystemComponent = nullptr;
	HealthSet = nullptr;
}

void UBattleHealthComponent::StartDeath()
{
	if (DeathState != EBattleDeathState::NotDead)
	{
		return;
	}

	DeathState = EBattleDeathState::DeathStarted;

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(FBattleGameplayTags::Get().Status_Death_Dying, 1);
	}

	AActor* Owner = GetOwner();
	check(Owner);

	OnDeathStarted.Broadcast(Owner);

	Owner->ForceNetUpdate();
}

void UBattleHealthComponent::FinishDeath()
{
	if (DeathState != EBattleDeathState::DeathStarted)
	{
		return;
	}

	DeathState = EBattleDeathState::DeathFinished;

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->SetLooseGameplayTagCount(FBattleGameplayTags::Get().Status_Death_Dead, 1);
	}

	AActor* Owner = GetOwner();
	check(Owner);

	OnDeathFinished.Broadcast(Owner);

	Owner->ForceNetUpdate();
}


static AActor* GetInstigatorFromAttrChangeData(const FOnAttributeChangeData& ChangeData)
{
	// GameEffectModifier에 Data가 있을 경우에만 호출
	if (ChangeData.GEModData != nullptr)
	{
		const FGameplayEffectContextHandle& EffectContext = ChangeData.GEModData->EffectSpec.GetEffectContext();
		return EffectContext.GetOriginalInstigator();
	}
	return nullptr;
}

void UBattleHealthComponent::HandleHealthChanged(const FOnAttributeChangeData& ChangeData)
{
	OnHealthChanged.Broadcast(this, ChangeData.OldValue, ChangeData.NewValue, GetInstigatorFromAttrChangeData(ChangeData));
}

void UBattleHealthComponent::HandleOutOfHealth(AActor* DamageInstigator, AActor* DamageCauser,
	const FGameplayEffectSpec& DamageEffectSpec, float DamageMagnitude)
{
	if (AbilitySystemComponent)
	{
		{
			FGameplayEventData Payload;
			Payload.EventTag = FBattleGameplayTags::Get().GameplayEvent_Death;
			Payload.Instigator = DamageInstigator;
			Payload.Target =AbilitySystemComponent->GetAvatarActor();
			Payload.OptionalObject = DamageEffectSpec.Def;
			Payload.ContextHandle = DamageEffectSpec.GetEffectContext();
			Payload.InstigatorTags = *DamageEffectSpec.CapturedSourceTags.GetAggregatedTags();
			Payload.TargetTags = *DamageEffectSpec.CapturedTargetTags.GetAggregatedTags();
			Payload.EventMagnitude = DamageMagnitude;

			FScopedPredictionWindow NewScopedWindow(AbilitySystemComponent, true);
			AbilitySystemComponent->HandleGameplayEvent(Payload.EventTag, &Payload);
		}

		{
			FBattleVerbMessage Message;
			Message.Verb = TAG_Battle_Elimination_Message;
			Message.Instigator = DamageInstigator;
			Message.InstigatorTags = *DamageEffectSpec.CapturedSourceTags.GetAggregatedTags();
			Message.Target = Cast<UObject>(UBattleVerbMessageHelpers::GetPlayerStateFromObject(AbilitySystemComponent->GetAvatarActor()));
			Message.TargetTags = *DamageEffectSpec.CapturedTargetTags.GetAggregatedTags();

			UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(GetWorld());
			MessageSubsystem.BroadcastMessage(Message.Verb, Message);
		}
	}
	
}

