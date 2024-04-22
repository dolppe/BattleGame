#include "BattleGameplayAbility_Jump.h"

#include "LyraBattleRoyalGame/Character/BattleCharacter.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleGameplayAbility_Jump)



UBattleGameplayAbility_Jump::UBattleGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UBattleGameplayAbility_Jump::CanActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                     const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags,
                                                     const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
	{
		return false;
	}

	const ABattleCharacter* BattleCharacter = Cast<ABattleCharacter>(ActorInfo->AvatarActor.Get());
	if (!BattleCharacter || !BattleCharacter->CanJump())
	{
		return false;
	}

	if (!Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags))
	{
		return false;
	}

	return true;
	
}

void UBattleGameplayAbility_Jump::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	CharacterJumpStop();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UBattleGameplayAbility_Jump::CharacterJumpStart()
{
	if (ABattleCharacter* BattleCharacter = GetBattleCharacterFromActorInfo())
	{
		if (BattleCharacter->IsLocallyControlled() && !BattleCharacter->bPressedJump)
		{
			BattleCharacter->UnCrouch();
			BattleCharacter->Jump();
		}
	}
	
}

void UBattleGameplayAbility_Jump::CharacterJumpStop()
{
	if (ABattleCharacter* BattleCharacter = GetBattleCharacterFromActorInfo())
	{
		if (BattleCharacter->IsLocallyControlled() && BattleCharacter->bPressedJump)
		{
			BattleCharacter->StopJumping();
		}
	}
}
