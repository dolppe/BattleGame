#pragma once

#include "BattleWeaponInstance.h"
#include "BattleRangedWeaponInstance.generated.h"

UCLASS()
class UBattleRangedWeaponInstance : public UBattleWeaponInstance
{
	GENERATED_BODY()
public:
	UBattleRangedWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// 유효 사거리
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WeaponConfig", meta = (ForceUnits = cm))
	float MaxDamageRange = 25000.0f;

	// 총탄 범위
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WeaponConfig", meta = (ForceUnits = cm))
	float BulletTraceWeaponRadius = 0.0f;
};

