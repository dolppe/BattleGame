#include "BattleAbilitySystemGlobals.h"
#include "BattleGameplayEffectContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleAbilitySystemGlobals)

UBattleAbilitySystemGlobals::UBattleAbilitySystemGlobals(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

FGameplayEffectContext* UBattleAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FBattleGameplayEffectContext();
}
