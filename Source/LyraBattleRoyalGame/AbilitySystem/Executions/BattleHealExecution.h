#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "BattleHealExecution.generated.h"

/*
 *	GameplayEffect의 Execution을 커스텀하여 GameplayEffect를 처리하는 것임.
 */

UCLASS()
class UBattleHealExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
public:
	UBattleHealExecution();

	// 이는 UGameplayEffectExecutionCalculation의 Execute BlueprintNativeEvent를 오버라이드 한것.
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
};

