#pragma once

#include "Abilities/GameplayAbilityTargetTypes.h"
#include "BattleGameplayAbilityTargetData_SingleTargetHit.generated.h"

USTRUCT()
struct FBattleGameplayAbilityTargetData_SingleTargetHit : public FGameplayAbilityTargetData_SingleTargetHit
{
	GENERATED_BODY()
public:

	FBattleGameplayAbilityTargetData_SingleTargetHit()
		: CartridgeID(-1)
	{}

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FBattleGameplayAbilityTargetData_SingleTargetHit::StaticStruct();
	}

	UPROPERTY()
	int32 CartridgeID;
	
};
