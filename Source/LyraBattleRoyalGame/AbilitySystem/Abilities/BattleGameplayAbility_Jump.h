#pragma once
#include "BattleGameplayAbility.h"

#include "BattleGameplayAbility_Jump.generated.h"


UCLASS(Abstract)
class UBattleGameplayAbility_Jump : public UBattleGameplayAbility
{
	GENERATED_BODY()
public:
	UBattleGameplayAbility_Jump(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category="Battle|Ability")
	void CharacterJumpStart();

	UFUNCTION(BlueprintCallable, Category="Battle|Ability")
	void CharacterJumpStop();
};
