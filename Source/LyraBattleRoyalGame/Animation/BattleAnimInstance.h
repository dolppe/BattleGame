#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Animation/AnimInstance.h"
#include "BattleAnimInstance.generated.h"

class UAbilitySystemComponent;

UCLASS()
class UBattleAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UBattleAnimInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());	
	
	virtual void NativeInitializeAnimation() override;

	void InitializeWithAbilitySystem(UAbilitySystemComponent* ASC);
	
	
	// 해당 속성값은 Lyra의 AnimBP에서 사용되는 값임 
	UPROPERTY(BlueprintReadOnly, Category="Character State Data")
	float GroundDistance = -1.0f;

	UPROPERTY(EditDefaultsOnly, Category="GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;
	
};
