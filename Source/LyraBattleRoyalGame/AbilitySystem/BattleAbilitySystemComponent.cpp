#include "BattleAbilitySystemComponent.h"

#include "BattleAbilityTagRelationshipMapping.h"
#include "Abilities/BattleGameplayAbility.h"
#include "LyraBattleRoyalGame/Animation/BattleAnimInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleAbilitySystemComponent)

UBattleAbilitySystemComponent::UBattleAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UBattleAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	FGameplayAbilityActorInfo* ActorInfo = AbilityActorInfo.Get();
	check(ActorInfo);
	check(InOwnerActor);

	const bool bHasNewPawnAvatar = Cast<APawn>(InAvatarActor) && (InAvatarActor != ActorInfo->AvatarActor);
	
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);

	if (bHasNewPawnAvatar)
	{
		if (UBattleAnimInstance* AnimInstance = Cast<UBattleAnimInstance>(ActorInfo->GetAnimInstance()))
		{
			AnimInstance->InitializeWithAbilitySystem(this);
		}
	}
	
}

void UBattleAbilitySystemComponent::AbilityInputTagPressed(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputPressedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.AddUnique(AbilitySpec.Handle);
			}
		}
	}
}

void UBattleAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if (InputTag.IsValid())
	{
		for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
		{
			if (AbilitySpec.Ability && (AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag)))
			{
				InputReleasedSpecHandles.AddUnique(AbilitySpec.Handle);
				InputHeldSpecHandles.Remove(AbilitySpec.Handle);
			}
		}
	}
}

void UBattleAbilitySystemComponent::ProcessAbilityInput(float DeltaTime, bool bGamePaused)
{
	TArray<FGameplayAbilitySpecHandle> AbilitiesToActive;

	for (const FGameplayAbilitySpecHandle& SpecHandle : InputHeldSpecHandles)
	{
		// Check Activatable Ability
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability && !AbilitySpec->IsActive())
			{
				const UBattleGameplayAbility* BattleAbilityCDO = CastChecked<UBattleGameplayAbility>(AbilitySpec->Ability);

				if (BattleAbilityCDO->GetActivationPolicy() == EBattleAbilityActivationPolicy::WhileInputActive)
				{
					AbilitiesToActive.AddUnique(AbilitySpec->Handle);
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : InputPressedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = true;

				if (AbilitySpec->IsActive())
				{
					// 이미 Ability가 활성화 되어있으면 InputEvent만
					AbilitySpecInputPressed(*AbilitySpec);
				}
				else
				{
					const UBattleGameplayAbility* BattleAbilityCDO = CastChecked<UBattleGameplayAbility>(AbilitySpec->Ability);

					if (BattleAbilityCDO->GetActivationPolicy() == EBattleAbilityActivationPolicy::OnInputTriggered)
					{
						AbilitiesToActive.AddUnique(AbilitySpec->Handle);
					}
				}
			}
		}
	}

	for (const FGameplayAbilitySpecHandle& AbilitySpecHandle : AbilitiesToActive)
	{
		TryActivateAbility(AbilitySpecHandle);
	}

	for (const FGameplayAbilitySpecHandle& SpecHandle : InputReleasedSpecHandles)
	{
		if (FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(SpecHandle))
		{
			if (AbilitySpec->Ability)
			{
				AbilitySpec->InputPressed = false;
				if (AbilitySpec->IsActive())
				{
					AbilitySpecInputReleased(*AbilitySpec);
				}
			}
		}
	}

	InputPressedSpecHandles.Reset();
	InputReleasedSpecHandles.Reset();
	
	
}

void UBattleAbilitySystemComponent::SetTagRelationshipMapping(UBattleAbilityTagRelationshipMapping* NewMapping)
{
	TagRelationshipMapping = NewMapping;
}

void UBattleAbilitySystemComponent::GetAdditionalActivationTagRequirements(const FGameplayTagContainer& AbilityTags,
                                                                           FGameplayTagContainer& OutActivationRequired, FGameplayTagContainer& OutActivationBlocked) const
{
	if (TagRelationshipMapping)
	{
		TagRelationshipMapping->GetRequiredAndBlockedActivationTags(AbilityTags, &OutActivationRequired, &OutActivationBlocked);
	}
}

void UBattleAbilitySystemComponent::AbilitySpecInputPressed(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputPressed(Spec);

	if (Spec.IsActive())
	{
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UBattleAbilitySystemComponent::AbilitySpecInputReleased(FGameplayAbilitySpec& Spec)
{
	Super::AbilitySpecInputReleased(Spec);

	if (Spec.IsActive())
	{
		InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, Spec.Handle, Spec.ActivationInfo.GetActivationPredictionKey());
	}
}

void UBattleAbilitySystemComponent::ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags,
                                                                   UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags,
                                                                   bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags)
{
	FGameplayTagContainer ModifiedBlockTags = BlockTags;
	FGameplayTagContainer ModifiedCancelTags = CancelTags;

	if (TagRelationshipMapping)
	{
		TagRelationshipMapping->GetAbilityTagsToBlockAndCancel(AbilityTags, &ModifiedBlockTags, &ModifiedCancelTags);
	}
	
	Super::ApplyAbilityBlockAndCancelTags(AbilityTags, RequestingAbility, bEnableBlockTags, ModifiedBlockTags,
	                                      bExecuteCancelTags, ModifiedCancelTags);
}
