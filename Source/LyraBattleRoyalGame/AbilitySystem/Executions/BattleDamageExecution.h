#pragma once

#include "GameplayEffectExecutionCalculation.h"
#include "BattleDamageExecution.generated.h"


UCLASS()
class UBattleDamageExecution : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:

	UBattleDamageExecution();

protected:

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
	
	
};

