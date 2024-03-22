#pragma once

#include "Abilities/GameplayAbility.h"
#include "BattleGameplayAbility.generated.h"

UENUM()
enum class EBattleAbilityActivationPolicy : uint8
{
	// Input이 Trigger 되었을 경우 실행 (Pressed/Released)
	OnInputTriggered,
	// Input이 Held 되어 있을 경우
	WhileInputActive,
	// Avatar가 생성되엇을 경우 바로 할당
	OnSpawn,
};


UCLASS(Abstract)
class UBattleGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UBattleGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Battle|AbilityActivation")
	EBattleAbilityActivationPolicy ActivationPolicy;

	
};
