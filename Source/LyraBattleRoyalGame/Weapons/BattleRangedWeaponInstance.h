#pragma once

#include "BattleWeaponInstance.h"
#include "BattleRangedWeaponInstance.generated.h"

UCLASS()
class UBattleRangedWeaponInstance : public UBattleWeaponInstance
{
	GENERATED_BODY()
public:
	UBattleRangedWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	int32 GetBulletsPerCartridge() const
	{
		return BulletsPerCartridge;
	}	
	
	float GetSpreadExponent() const
	{
		return SpreadExponent;
	}

	float GetCalculatedSpreadAngle() const
	{
		return TempSpreadAngle;
	}

	float GetCalculatedSpreadAngleMultiplier() const
	{
		return TempSpreadAngleMultiplier;
	}

	float GetMaxDamageRange() const
	{
		return MaxDamageRange;
	}

	float GetBulletTraceSweepRadius() const
	{
		return BulletTraceSweepRadius;
	}
	

protected:
	
	// 유효 사거리
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WeaponConfig", meta = (ForceUnits = cm))
	float MaxDamageRange = 25000.0f;

	// 총탄 범위
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WeaponConfig", meta = (ForceUnits = cm))
	float BulletTraceSweepRadius = 0.0f;

	// 중앙선을 기준으로 확산 정도 => 1.0이 제일 정확한 것.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0.1), Category="Spread|Fire Params")
	float SpreadExponent = 1.0f;
	
	// Single 카드리지에 나가는 총알 수 (일반적으로는 1이고, 샷건은 더 나감)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Config")
	int32 BulletsPerCartridge = 1;

	
	// Heat 관련 (연사)해서 각 Spread 정도에 관한 변수들 추후 추가할것.
	// 움직임 관련해서 Spread 처리

	/*
	 * 연사에 대한 탄퍼짐을 넣기 전 테스트용 탄퍼짐
	 */
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spread|Fire Params")
	float TempSpreadAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spread|Fire Params")
	float TempSpreadAngleMultiplier;
	
private:

	
	// 현재 Heat나 여러가지에 따라서 결정되는 탄퍼짐 정도 (Degree로 표현됨)
	// 이걸로 원뿔을 그려서 탄퍼짐 정도가 결정됨.
	float CurrentSpreadAngle = 0.0f;

	// The current *combined* spread angle multiplier
	// 움직임과 같은 상황에 따라서 기존의 CurrentSpreadAngle에 곱해져서 가중치를 더하기 위한 값.
	float CurrentSpreadAngleMultiplier = 1.0f;
	
	
};

