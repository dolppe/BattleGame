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

class UBattleAbilityCost;

UCLASS(Abstract)
class UBattleGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UBattleGameplayAbility(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ApplyCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Battle|AbilityActivation")
	EBattleAbilityActivationPolicy ActivationPolicy;

	UPROPERTY(EditDefaultsOnly, Instanced, Category=Costs)
	TArray<TObjectPtr<UBattleAbilityCost>> AdditionalCosts;
	
};
