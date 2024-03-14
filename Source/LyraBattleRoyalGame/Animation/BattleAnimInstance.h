#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BattleAnimInstance.generated.h"


UCLASS()
class UBattleAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UBattleAnimInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(BlueprintReadOnly, Category="Character State Data")
	float GroundDistance = -1.0f;
};

