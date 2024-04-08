#pragma once

#include "CoreMinimal.h"
#include "BattleGameplayAbility.h"
#include "GameplayAbilitySpec.h"
#include "BattleAbilityCost.generated.h"

UCLASS(DefaultToInstanced, EditInlineNew, Abstract)
class UBattleAbilityCost : public UObject
{
	GENERATED_BODY()
public:

	UBattleAbilityCost();

	virtual bool CheckCost(const UBattleGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
	{
		return true;
	}

	virtual void ApplyCost(const UBattleGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
	{
	}
	
};

