#include "BattleGameplayAbility_Death.h"

#include "GameplayTagsManager.h"
#include "LyraBattleRoyalGame/BattleGameplayTags.h"
#include "LyraBattleRoyalGame/AbilitySystem/BattleAbilitySystemComponent.h"
#include "LyraBattleRoyalGame/Character/BattleHealthComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleGameplayAbility_Death)


UBattleGameplayAbility_Death::UBattleGameplayAbility_Death(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;

	UGameplayTagsManager::Get().CallOrRegister_OnDoneAddingNativeTagsDelegate(FSimpleDelegate::CreateUObject(this, &ThisClass::DoneAddingNativeTags));
}

void UBattleGameplayAbility_Death::DoneAddingNativeTags()
{
	if (HasAnyFlags(RF_ClassDefaultObject))
	{
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = FBattleGameplayTags::Get().GameplayEvent_Death;
		TriggerData.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent;
		AbilityTriggers.Add(TriggerData);
	}
}

void UBattleGameplayAbility_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	check(ActorInfo);

	UBattleAbilitySystemComponent* BattleASC = CastChecked<UBattleAbilitySystemComponent>(ActorInfo->AbilitySystemComponent.Get());

	FGameplayTagContainer AbilityTypesToIgnore;
	AbilityTypesToIgnore.AddTag(FBattleGameplayTags::Get().Ability_Behavior_SurvivesDeath);

	BattleASC->CancelAbilities(nullptr, &AbilityTypesToIgnore, this);
	
	SetCanBeCanceled(false);

	StartDeath();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UBattleGameplayAbility_Death::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	check(ActorInfo);

	FinishDeath();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UBattleGameplayAbility_Death::StartDeath()
{
	if (UBattleHealthComponent* HealthComponent = UBattleHealthComponent::FindHealthComponent(GetAvatarActorFromActorInfo()))
	{
		if (HealthComponent->GetDeathState() == EBattleDeathState::NotDead)
		{
			HealthComponent->StartDeath();
		}
	}
}

void UBattleGameplayAbility_Death::FinishDeath()
{
		if (UBattleHealthComponent* HealthComponent = UBattleHealthComponent::FindHealthComponent(GetAvatarActorFromActorInfo()))
    	{
    		if (HealthComponent->GetDeathState() == EBattleDeathState::DeathStarted)
    		{
    			HealthComponent->FinishDeath();
    		}
    	}
}
