#pragma once

#include "GameplayAbilitySpec.h"
#include "BattleGameplayAbility.h"
#include "BattleGameplayAbility_Death.generated.h"

UCLASS()
class UBattleGameplayAbility_Death : public UBattleGameplayAbility
{
	GENERATED_BODY()
public:

	UBattleGameplayAbility_Death(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	
	void DoneAddingNativeTags();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	UFUNCTION(BlueprintCallable, Category="Battle|Ability")
	void StartDeath();

	UFUNCTION(BlueprintCallable, Category="Battle|Ability")
	void FinishDeath();
	
};

