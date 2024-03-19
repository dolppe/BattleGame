#pragma once

#include "Abilities/GameplayAbility.h"
#include "BattleGameplayAbility.generated.h"


UCLASS(Abstract)
class UBattleGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UBattleGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
