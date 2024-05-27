#pragma once

#include "BattleAbilitySourceInterface.h"
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

	void SetAbilitySource(const IBattleAbilitySourceInterface* InObject, float InSourceLevel);
	const IBattleAbilitySourceInterface* GetAbilitySource() const;

	const UPhysicalMaterial* GetPhysicalMaterial() const;

	virtual FGameplayEffectContext* Duplicate() const override
	{
		FBattleGameplayEffectContext* NewContext = new FBattleGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FBattleGameplayEffectContext::StaticStruct();
	}

	
protected:
	// IBattleAbilitySourceObject
	UPROPERTY()
	TWeakObjectPtr<const UObject> AbilitySourceObject;
	
};
