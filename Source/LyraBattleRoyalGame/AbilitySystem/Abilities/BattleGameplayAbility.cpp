#include "BattleGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "BattleAbilityCost.h"
#include "LyraBattleRoyalGame/BattleGameplayTags.h"
#include "LyraBattleRoyalGame/AbilitySystem/BattleAbilitySystemComponent.h"
#include "LyraBattleRoyalGame/Character/BattleCharacter.h"
#include "LyraBattleRoyalGame/Player/BattlePlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleGameplayAbility)

UBattleGameplayAbility::UBattleGameplayAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ActivationPolicy = EBattleAbilityActivationPolicy::OnInputTriggered;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

UBattleAbilitySystemComponent* UBattleGameplayAbility::GetBattleAbilitySystemComponentFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<UBattleAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent) : nullptr);
}

ABattlePlayerController* UBattleGameplayAbility::GetBattlePlayerControllerFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ABattlePlayerController>(CurrentActorInfo->PlayerController.Get()) : nullptr);
}

AController* UBattleGameplayAbility::GetControllerFromActorInfo() const
{
	if (CurrentActorInfo)
	{
		if (AController* PC = CurrentActorInfo->PlayerController.Get())
		{
			return PC;
		}

		AActor* FindActor = CurrentActorInfo->OwnerActor.Get();
		while (FindActor)
		{
			if (AController* Controller = Cast<AController>(FindActor))
			{
				return Controller;
			}

			if (APawn* Pawn = Cast<APawn>(FindActor))
			{
				return Pawn->GetController();
			}

			FindActor = FindActor->GetOwner();
		}
	}
	return nullptr;
}

ABattleCharacter* UBattleGameplayAbility::GetBattleCharacterFromActorInfo() const
{
	return (CurrentActorInfo ? Cast<ABattleCharacter>(CurrentActorInfo->AvatarActor.Get()) : nullptr);
}


void UBattleGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	K2_OnAbilityAdded();
}

void UBattleGameplayAbility::OnRemoveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	K2_OnAbilityRemoved();
	
	Super::OnRemoveAbility(ActorInfo, Spec);
}

bool UBattleGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags))
	{
		return false;
	}

	for (TObjectPtr<UBattleAbilityCost> AdditionalCost : AdditionalCosts)
	{
		if (AdditionalCost != nullptr)
		{
			if (!AdditionalCost->CheckCost(this, Handle, ActorInfo, OptionalRelevantTags))
			{
				return false;
			}
		}
	}

	return true;
}


void UBattleGameplayAbility::ApplyCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const
{
	Super::ApplyCost(Handle, ActorInfo, ActivationInfo);

	check(ActorInfo);

	for (TObjectPtr<UBattleAbilityCost> AdditionalCost : AdditionalCosts)
	{
		if (AdditionalCost != nullptr)
		{
			AdditionalCost->ApplyCost(this, Handle, ActorInfo, ActivationInfo);
		}
	}
}

bool UBattleGameplayAbility::DoesAbilitySatisfyTagRequirements(const UAbilitySystemComponent& AbilitySystemComponent,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	bool bBlocked = false;
	bool bMissing = false;

	UAbilitySystemGlobals& AbilitySystemGlobals = UAbilitySystemGlobals::Get();
	const FGameplayTag& BlockedTag = AbilitySystemGlobals.ActivateFailTagsBlockedTag;
	const FGameplayTag& MissingTag = AbilitySystemGlobals.ActivateFailTagsMissingTag;

	if (AbilitySystemComponent.AreAbilityTagsBlocked(AbilityTags))
	{
		bBlocked = true;
	}

	const UBattleAbilitySystemComponent* BattleASC = Cast<UBattleAbilitySystemComponent>(&AbilitySystemComponent);
	static FGameplayTagContainer AllRequiredTags;
	static FGameplayTagContainer AllBlockedTags;

	AllRequiredTags = ActivationRequiredTags;
	AllBlockedTags = ActivationBlockedTags;

	if (BattleASC)
	{
		BattleASC->GetAdditionalActivationTagRequirements(AbilityTags, AllRequiredTags, AllBlockedTags);
	}
	
	if (AllBlockedTags.Num() || AllRequiredTags.Num())
	{
		static FGameplayTagContainer AbilitySystemComponentTags;

		AbilitySystemComponentTags.Reset();
		AbilitySystemComponent.GetOwnedGameplayTags(AbilitySystemComponentTags);

		if (AbilitySystemComponentTags.HasAny(AllBlockedTags))
		{
			const FBattleGameplayTags& GameplayTags = FBattleGameplayTags::Get();

			bBlocked = true;
		}

		if (!AbilitySystemComponentTags.HasAll(AllRequiredTags))
		{
			bMissing = true;
		}
	}

	if (SourceTags != nullptr)
	{
		if (SourceBlockedTags.Num() || SourceRequiredTags.Num())
		{
			if (SourceTags->HasAny(SourceBlockedTags))
			{
				bBlocked = true;
			}

			if (!SourceTags->HasAll(SourceRequiredTags))
			{
				bMissing = true;
			}
		}
	}

	if (TargetTags != nullptr)
	{
		if (TargetBlockedTags.Num() || TargetRequiredTags.Num())
		{
			if (TargetTags->HasAny(TargetBlockedTags))
			{
				bBlocked = true;
			}

			if (!TargetTags->HasAll(TargetRequiredTags))
			{
				bMissing = true;
			}
		}
	}

	if (bBlocked)
	{
		if (OptionalRelevantTags && BlockedTag.IsValid())
		{
			OptionalRelevantTags->AddTag(BlockedTag);
		}
		return false;
	}
	
	if (bMissing)
	{
		if (OptionalRelevantTags && MissingTag.IsValid())
		{
			OptionalRelevantTags->AddTag(MissingTag);
		}
		return false;
	}
	
	return true;
}
