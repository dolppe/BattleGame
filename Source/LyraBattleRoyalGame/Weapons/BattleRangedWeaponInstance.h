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
		return CurrentSpreadAngle;
	}

	float GetCalculatedSpreadAngleMultiplier() const
	{
		return bHasFirstShotAccuracy ? 0.0f : CurrentSpreadAngleMultiplier;
	}

	bool HasFirstShotAccuracy() const
	{
		return bHasFirstShotAccuracy;
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
	


	// 중앙선을 기준으로 확산 정도 => 1.0이 제일 정확한 것.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0.1), Category="Spread|Fire Params")
	float SpreadExponent = 1.0f;

	/*
	 * Fire Params는 총기 발사에 대한 파라미터들을 모아둠.
	 * 주로 Heat의 값을 정하고, 이에 따라서 Spared 값을 정해서 현재 샷의 확산 정도를 정함.
	 */
	
	// X => heat / Y => Spread Angle
	UPROPERTY(EditAnywhere, Category="Spread|Fire Params")
	FRuntimeFloatCurve HeatToSpreadCurve;

	// 현재의 Heat값에 따라서 Shot마다 올라갈 Heat의 양을 정할 수 있음.
	// X => Heat / Y => 다음 Single Shot에 올라갈 Heat값.
	// 과열 정도에 따라서 더 빠르게 과열되게 하는 등의 작업을 할 수 있음.
	UPROPERTY(EditAnywhere, Category="Spread|Fire Params")
	FRuntimeFloatCurve HeatToHeatPerShotCurve;

	// Heat 값에 따라서 초당 CoolDown될 값을 의미함.
	// 일반적으로는 모두 같은 값으로, 얼마나 빠르게 Heat가 식을지를 나타냄.
	// X => Heat / Y => Cooldown Value (Per Sec)
	// 보통은 평평한 직선으로 하지만, 이를 건드리면 과열됐을 때 잘 안식게 만들수도 있음.
	UPROPERTY(EditAnywhere, Category="Spread|Fire Params")
	FRuntimeFloatCurve HeatToCoolDownPerSecondCurve;

	// 총을 발사한 후에 확산값이 회복되기를 시작하기까지 걸리는 시간 (초단위)
	UPROPERTY(EditAnywhere, Category="Spread|Fire Params", meta=(ForceUnits=s))
	float SpreadRecoveryCooldownDelay = 0.0f;

	// 첫 번째 샷은 정확하게 나가도록 해줌. 최소 Spared값으로
	UPROPERTY(EditAnywhere, Category="Spread|Fire Params")
	bool bAllowFirstShotAccuracy = false;

	/*
	 * Player Params는 현재 사용자의 상태에 따라서 Spread에 곱해질 가중치값을 나타냄.
	 */

	// Aiming하고 있을 때의 확산 가중치
	UPROPERTY(EditAnywhere, Category="Spread|Player Params", meta=(ForceUnits=x))
	float SpreadAngleMultiplier_Aiming = 1.0f;

	// 느리게 움직이거나 정지하여 계속 서있을 때의 확산 가중치
	// 실제 정지 상태일 때 적용할 가중치 (보통은 1보다 낮게 해서 확산 정도를 낮춤)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spread|Player Params", meta=(ForceUnits=x))
	float SpreadAngleMultiplier_StandingStill = 1.0f;

	// 정지 상태로 전이되거나 정지 상태에서 다른 상태로 전이될 때의 정확도 비율
	// 높은 값을 가질수록 전이가 빠르게 됨. 그러나 0의 값을 가지면 바로 전이됨.
	// 정지 가중치가 변하는 속도
	// 일반적으로 정지면 1보다는 낮은 가중치를 가지는데, 현재의 가중치 => SpreadAngleMultiplier_StandingStill(정지가중치)까지
	// 얼마나 빠르게 변할지 (반대로 정지에서 다른 것으로 갈때에도 변환 속도에 영향)ㄴ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spread|Player Params")
	float TransitionRate_StandingStill = 5.0f;

	// 완전한 정지 상태로 여겨지는 최대 속도
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spread|Player Params", meta=(ForceUnits="cm/s"))
	float StandingStillSpeedThreshold = 80.0f;

	// 완전한 정지는 아니지만, StandingStillSpeedThreshold + StandingStillToMovingSpeedRange 값까지는
	// 어느 정도 정지 가중치를 준다는 뜻
	// ex) Default 값으로는 80이하면 정지상태이고, 온전한 정지 가중치 (SpreadAngleMultiplier_StandingStill)를 가짐
	// 그러나 90이면, 아직 정지 상태는 아니고, 이를 SpreadAngleMultiplier_StandingStill ~ 1.0f의 중간인 정지 가중치를 가지게 되는 것.
	// 100을 넘어가면 그냥 1.0f로 정지에 대해서는 가중치 영향이 없는 것임.
	// 만약 속도가 90 => 80으로 변해서 가중치가 더 낮아져야 한다면, 이는 TrasitionRate_StandingStill의 속도 정도로 변함.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spread|Player Params", meta=(ForceUnits="cm/s"))
	float StandingStillToMovingSpeedRange = 20.0f;

	// 움크린 상태에서의 확산 가중치
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spread|Player Params", meta=(ForceUnits=x))
	float SpreadAngleMultiplier_Crouching = 1.0f;

	// 움크린 상태로의 전이나 움크린 상태에서 다른 상태로 전이되는 정확도 비율
	// 높은 값을 가질 수록 더 빠르게 전이됨. 그러나 0의 값을 가지면 바로 전이됨.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spread|Player Params")
	float TransitionRate_Crouching = 5.0f;

	// 점프하거나 떨어질 때의 확산 가중치
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spread|Player Params", meta=(ForceUnits=x))
	float SpreadAngleMultiplier_JumpingOrFalling = 1.0f;

	// 점프나 떨어지는 상태에 관한 전이에 대한 정확도 비율
	// 높은 값을 가질수록 빠르게 전이, 0이면 바로 전이
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spread|Player Params")
	float TransitionRate_JumpingOrFalling = 5.0f;

	/*
	 * 총기 스펙
	 */
	
	// Single 카드리지에 나가는 총알 수 (일반적으로는 1이고, 샷건은 더 나감)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WeaponConfig")
	int32 BulletsPerCartridge = 1;

	// 유효 사거리
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WeaponConfig", meta = (ForceUnits = cm))
	float MaxDamageRange = 25000.0f;

	// 총탄 범위
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="WeaponConfig", meta = (ForceUnits = cm))
	float BulletTraceSweepRadius = 0.0f;

	
private:

	// 마지막 총 발사 시간
	double LastFireTime = 0.0;

	// 현재 Heat 값
	float CurrentHeat = 0.0f;

	// 현재 Heat나 여러가지에 따라서 결정되는 탄퍼짐 정도 (Degree로 표현됨)
	// 이걸로 원뿔을 그려서 탄퍼짐 정도가 결정됨.
	float CurrentSpreadAngle = 0.0f;

	// 첫 번째 샷 정확도를 가지고 있는지 여부
	bool bHasFirstShotAccuracy = false;

	// 움직임과 같은 상황에 따라서 기존의 CurrentSpreadAngle에 곱해져서 가중치를 더하기 위한 값.
	// 각종 가중치들을 곱해서 가짐.
	float CurrentSpreadAngleMultiplier = 1.0f;
	
	// 정지에 관한 현재 가중치
	float StandingStillMultiplier = 1.0f;

	// 점프나 떨어지는 것에 대한 현재 가중치
	float JumpFallMultiplier = 1.0f;

	// 움크리고 있는 것에 대한 현재 가중치
	float CrouchingMultiplier = 1.0f;


public:

	void Tick(float DeltaSeconds);
	
	/*
	 * EquipmentInstance Interface
	 */
	virtual void OnEquipped() override;

	void AddSpared();

private:
	void ComputeSpreadRange(float& MinSpread, float& MaxSpread);
	void ComputeHeatRange(float& MinHeat, float& MaxHeat);

	inline float ClampHeat(float NewHeat)
	{
		float MinHeat;
		float MaxHeat;
		ComputeHeatRange(MinHeat, MaxHeat);

		return FMath::Clamp(NewHeat, MinHeat, MaxHeat);
	}

	bool UpdateSpread(float DeltaSeconds);
	bool UpdateMultipliers(float DeltaSeconds);
	
	
	
	
};

