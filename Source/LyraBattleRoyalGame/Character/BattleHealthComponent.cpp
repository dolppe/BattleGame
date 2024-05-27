#include "BattleHealthComponent.h"
#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"
#include "LyraBattleRoyalGame/BattleLogChannels.h"
#include "LyraBattleRoyalGame/AbilitySystem/BattleAbilitySystemComponent.h"
#include "LyraBattleRoyalGame/AbilitySystem/Attributes/BattleHealthSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleHealthComponent)

UBattleHealthComponent::UBattleHealthComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
		// 이벤트기반으로 Character와 PlayerState의 HealthSet의 브릿지 역할만 해줌.
    	// 따라서 틱이 필요가 없음.
    	PrimaryComponentTick.bStartWithTickEnabled = true;
    	PrimaryComponentTick.bCanEverTick = true;
    
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

	OnHealthChanged.Broadcast(this, HealthSet->GetHealth(), HealthSet->GetHealth(), nullptr);
	
}

void UBattleHealthComponent::UnInitializeWithAbilitySystem()
{
	AbilitySystemComponent = nullptr;
	HealthSet = nullptr;
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

void UBattleHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (HealthSet != nullptr)
	{
		UE_LOG(LogBattle, Log, TEXT("Health: %f"), HealthSet->GetHealth());
	}
}
