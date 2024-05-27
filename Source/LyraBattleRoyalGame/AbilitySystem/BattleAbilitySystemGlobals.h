#pragma once

#include "AbilitySystemGlobals.h"
#include "UObject/UObjectGlobals.h"
#include "BattleAbilitySystemGlobals.generated.h"

struct FGameplayEffectContext;

UCLASS(Config=Game)
class UBattleAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_UCLASS_BODY()

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
	
};

