#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BattleAnimInstance.generated.h"

class UAbilitySystemComponent;

UCLASS()
class UBattleAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UBattleAnimInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());	
	
	// 해당 속성값은 Lyra의 AnimBP에서 사용되는 값임 
	UPROPERTY(BlueprintReadOnly, Category="Character State Data")
	float GroundDistance = -1.0f;
	
};
