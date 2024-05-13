#pragma once

#include "GameplayEffectTypes.h"
#include "BattleGameplayEffectContext.generated.h"


class IBattleAbilitySourceInterface;

USTRUCT()
struct FBattleGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

	FBattleGameplayEffectContext()
		: FGameplayEffectContext()
	{
	}
	FBattleGameplayEffectContext(AActor* InInstigator, AActor* InEffectCauser)
		: FGameplayEffectContext(InInstigator, InEffectCauser)
	{
	}

	static LYRABATTLEROYALGAME_API FBattleGameplayEffectContext* ExtractEffectContext(struct FGameplayEffectContextHandle Handle);

	const IBattleAbilitySourceInterface* GetAbilitySource() const;

	const UPhysicalMaterial* GetPhysicalMaterial() const;

protected:
	UPROPERTY()
	TWeakObjectPtr<const UObject> AbilitySourceObject;
	
};
