#include "BattleHealExecution.h"

#include "LyraBattleRoyalGame/AbilitySystem/Attributes/BattleCombatSet.h"
#include "LyraBattleRoyalGame/AbilitySystem/Attributes/BattleHealthSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleHealExecution)

/*
 *	FGameplayEffectAttributeCaptureDefinition을 인스턴스화하여 전달함.
 */

struct FHealStatics
{
	// AttributeSet의 어떤 Attribute를 Capture할 것인지와 어떻게 Capture할 것인지 정의함.
	FGameplayEffectAttributeCaptureDefinition BaseHealDef;

	FHealStatics()
	{
		BaseHealDef = FGameplayEffectAttributeCaptureDefinition(UBattleCombatSet::GetBaseHealAttribute(), EGameplayEffectAttributeCaptureSource::Source, true);
	}
	
};

static FHealStatics& HealStatics()
{
	// 한번만 생성하고, 재사용
	static FHealStatics Statics;
	return Statics;
}

UBattleHealExecution::UBattleHealExecution()
	: Super()
{
	// Source로 Attribute 캡처를 정의함
	// CombatSet::BaseHeal을 통해 Healing값을 정의하고 최종 Execute때
	// 해당 값을 가져와서 Health에 Healing을 적용
	RelevantAttributesToCapture.Add(HealStatics().BaseHealDef);
}

void UBattleHealExecution::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	float BaseHeal = 0.0f;
	{
		FAggregatorEvaluateParameters EvaluateParameters;

		// 해당 함수 호출을 통해 CombatSet의 BaseHeal을 가져옴.
		// 만약 Modifier에 값이 있었으면 최종 결과 값이 나옴
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(HealStatics().BaseHealDef, EvaluateParameters, BaseHeal);
	}

	// 최종 계산된 BaseHeal을 0.0이하가 되지 않도록 제한
	const float HealingDone = FMath::Max(0.0f, BaseHeal);
	if (HealingDone>0.0f)
	{
		// Modifer로서 추가함
		// 가져온 BaseHeal을 통해서 HealthSet에 Healing에 추가해줌.
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UBattleHealthSet::GetHealingAttribute(), EGameplayModOp::Additive, HealingDone));
	}
	
}
