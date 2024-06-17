#include "BattleRangedWeaponInstance.h"

#include "NativeGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LyraBattleRoyalGame/BattleLogChannels.h"
#include "LyraBattleRoyalGame/Camera/BattleCameraComponent.h"
#include "LyraBattleRoyalGame/Physics/PhysicalMaterialWithTags.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleRangedWeaponInstance)

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_Battle_Weapon_SteadyAimingCamera, "Battle.Weapon.SteadyAimingCamera");

UBattleRangedWeaponInstance::UBattleRangedWeaponInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HeatToHeatPerShotCurve.EditorCurveData.AddKey(0.0f, 1.0f);
	HeatToCoolDownPerSecondCurve.EditorCurveData.AddKey(0.0f, 2.0f);
}

void UBattleRangedWeaponInstance::Tick(float DeltaSeconds)
{
	APawn* Pawn = GetPawn();
	check (Pawn != nullptr);

	const bool bMinSpread = UpdateSpread(DeltaSeconds);
	const bool bMinMultipliers = UpdateMultipliers(DeltaSeconds);

	bHasFirstShotAccuracy = bAllowFirstShotAccuracy && bMinMultipliers && bMinSpread;
}

void UBattleRangedWeaponInstance::OnEquipped()
{
	Super::OnEquipped();

	float MinHeatRange;
	float MaxHeatRange;
	ComputeHeatRange(MinHeatRange, MaxHeatRange);
	CurrentHeat = (MinHeatRange + MaxHeatRange) * 0.5f;

	CurrentSpreadAngle = HeatToSpreadCurve.GetRichCurveConst()->Eval(CurrentHeat);

	CurrentSpreadAngleMultiplier = 1.0f;
	StandingStillMultiplier = 1.0f;
	JumpFallMultiplier = 1.0f;
	CrouchingMultiplier = 1.0f;	
}

float UBattleRangedWeaponInstance::GetDistanceAttenuation(float Distance, const FGameplayTagContainer* SourceTags,
	const FGameplayTagContainer* TargetTags) const
{
	const FRichCurve* Curve = DistanceDamageFalloff.GetRichCurveConst();
	return Curve->HasAnyData() ? Curve->Eval(Distance) : 1.0f;
}

float UBattleRangedWeaponInstance::GetPhysicalMaterialAttenuation(const UPhysicalMaterial* PhysicalMaterial,
	const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags) const
{
	float CombinedMultiplier = 1.0f;
	if (const UPhysicalMaterialWithTags* PhysMatWithTags = Cast<const UPhysicalMaterialWithTags>(PhysicalMaterial))
	{
		for (const FGameplayTag MaterialTag : PhysMatWithTags->Tags)
		{
			if (const float* pTagMultiplier = MaterialDamageMultiplier.Find(MaterialTag))
			{
				CombinedMultiplier *= *pTagMultiplier;
			}
		}
	}

	return CombinedMultiplier;
}


void UBattleRangedWeaponInstance::AddSpared()
{
	const float HeatPerShot = HeatToHeatPerShotCurve.GetRichCurveConst()->Eval(CurrentHeat);
	CurrentHeat = ClampHeat(CurrentHeat + HeatPerShot);

	CurrentSpreadAngle = HeatToSpreadCurve.GetRichCurveConst()->Eval(CurrentHeat);
	
}

void UBattleRangedWeaponInstance::ComputeSpreadRange(float& MinSpread, float& MaxSpread)
{
	HeatToSpreadCurve.GetRichCurveConst()->GetValueRange(/*out*/ MinSpread, /*out*/ MaxSpread);
}

void UBattleRangedWeaponInstance::ComputeHeatRange(float& MinHeat, float& MaxHeat)
{
	float Min1;
	float Max1;
	HeatToHeatPerShotCurve.GetRichCurveConst()->GetTimeRange(/*out*/ Min1, /*out*/ Max1);

	float Min2;
	float Max2;
	HeatToCoolDownPerSecondCurve.GetRichCurveConst()->GetTimeRange(/*out*/ Min2, /*out*/ Max2);

	float Min3;
	float Max3;
	HeatToSpreadCurve.GetRichCurveConst()->GetTimeRange(/*out*/ Min3, /*out*/ Max3);

	MinHeat = FMath::Min(FMath::Min(Min1, Min2), Min3);
	MaxHeat = FMath::Max(FMath::Max(Max1, Max2), Max3);
}

bool UBattleRangedWeaponInstance::UpdateSpread(float DeltaSeconds)
{
	const float TimeSinceFired = GetWorld()->TimeSince(LastFireTime);

	if (TimeSinceFired > SpreadRecoveryCooldownDelay)
	{
		const float CooldownRate = HeatToCoolDownPerSecondCurve.GetRichCurveConst()->Eval(CurrentHeat);
		CurrentHeat = ClampHeat(CurrentHeat - (CooldownRate * DeltaSeconds));
		CurrentSpreadAngle = HeatToSpreadCurve.GetRichCurveConst()->Eval(CurrentHeat);
	}

	float MinSpread;
	float MaxSpread;
	ComputeSpreadRange(MinSpread, MaxSpread);

	return FMath::IsNearlyEqual(CurrentSpreadAngle, MinSpread, KINDA_SMALL_NUMBER);
}

bool UBattleRangedWeaponInstance::UpdateMultipliers(float DeltaSeconds)
{
	const float MultiplierNearlyEqualThreshold = 0.05f;

	APawn* Pawn = GetPawn();
	check(Pawn != nullptr);

	UCharacterMovementComponent* CharacterMovementComponent = Cast<UCharacterMovementComponent>(Pawn->GetMovementComponent());

	const float PawnSpeed = Pawn->GetVelocity().Size();
	// Value를 InputRange 안쪽으로 Clamp하고, 이에 대해서 OutputRange 백분율로 반환해줌.
	// 즉, Pawn의 Speed에 대해, 정지 가중치를 얼마나 적용할 지 정하는 로직
	// 우선 속도가 [StandingStillSpeedThreshold ~ StandingStillSpeedThreshold + StandingStillToMovingSpeedRange] 사이인지 Clamp 진행
	// 나온 결과에 따라서 만약 최소값인 StandingStillSpeedThreshold이 나오면, 이는 정지 상태로 간주하고, SpreadAngleMultiplier_StandingStill 정지 가중치 값을 반환함
	// 만약 속도가 기준치를 넘었으면, 이는 정지 상태가 아니므로 정지 가중치를 적용하지 않는 1.0f를 반환함
	// 만약 그 사이라면, 속도에 따라서 약간의 정지 가중치를 적용함
	const float MovementTargetValue = FMath::GetMappedRangeValueClamped(
		/*InputRange*/ FVector2D(StandingStillSpeedThreshold, StandingStillSpeedThreshold + StandingStillToMovingSpeedRange),
		/*OutputRange*/ FVector2D(SpreadAngleMultiplier_StandingStill, 1.0f),
		PawnSpeed);
	// Current에서 Target으로 보간
	// 한번에 정지 가중치가 변경되면, 어색할 수 있기 때문에 TransitionRate_StandingStill 속도 만큼 천천히 보간하면서 변하게 함.
	StandingStillMultiplier = FMath::FInterpTo(StandingStillMultiplier, MovementTargetValue, DeltaSeconds, TransitionRate_StandingStill);
	// 정지 가중치를 받고 있는지 체크하는 것 (이게 true면, 정지 상태라는 것임)
	const bool bStandingStillMultiplierAtMin = FMath::IsNearlyEqual(StandingStillMultiplier, SpreadAngleMultiplier_StandingStill, SpreadAngleMultiplier_StandingStill*0.1f);

	const bool bIsCrouching = (CharacterMovementComponent != nullptr) && CharacterMovementComponent->IsCrouching();
	// 만약 움크리고 있으면 움크리는 것에 대한 가중치를 적용 아니면 1.0으로 가중치 적용을 안함
	const float CrouchingTargetValue = bIsCrouching ? SpreadAngleMultiplier_Crouching : 1.0f;
	// 움크리기 가중치 적용
	CrouchingMultiplier = FMath::FInterpTo(CrouchingMultiplier, CrouchingTargetValue, DeltaSeconds, TransitionRate_Crouching);
	// 이게 true라는 것은 움크리기에 대해 변경사항이 없다는 뜻임
	const bool bCrouchingMultiplierAtTarget = FMath::IsNearlyEqual(CrouchingMultiplier, CrouchingTargetValue, MultiplierNearlyEqualThreshold);

	const bool bIsJumpingOrFalling = (CharacterMovementComponent!=nullptr) && CharacterMovementComponent->IsFalling();
	const float JumpFallTargetValue = bIsJumpingOrFalling ? SpreadAngleMultiplier_JumpingOrFalling : 1.0f;
	JumpFallMultiplier = FMath::FInterpTo(JumpFallMultiplier, JumpFallTargetValue, DeltaSeconds, TransitionRate_JumpingOrFalling);
	// 이게 true라는 것은 점프나 떨어지는 상태가 아니라는 뜻임
	const bool bJumpFallMultiplierIs = FMath::IsNearlyEqual(JumpFallMultiplier, 1.0f, MultiplierNearlyEqualThreshold);

	/*
	 * 아직 Aiming 카메라 모드를 만들지 않았기에 이의 가중치는 추후 적용
	 */
	float AimingAlpha = 0.0f;
	if (const UBattleCameraComponent* CameraComponent = UBattleCameraComponent::FindCameraComponent(Pawn))
	{
		float TopCameraWeight;
		FGameplayTag TopCameraTag;
		CameraComponent->GetBlendInfo(TopCameraWeight, TopCameraTag);

		AimingAlpha = ((TopCameraTag == TAG_Battle_Weapon_SteadyAimingCamera) ? TopCameraWeight :0.0f);

	}

	const float AimingMultiplier = FMath::GetMappedRangeValueClamped(
		FVector2D(0.0f, 1.0f),
		FVector2D(1.0f, SpreadAngleMultiplier_Aiming),
		AimingAlpha);
	const bool bAimingMultiplierAtTarget = FMath::IsNearlyEqual(AimingMultiplier, SpreadAngleMultiplier_Aiming, KINDA_SMALL_NUMBER);

	const float CombinedMultiplier = AimingMultiplier * StandingStillMultiplier * CrouchingMultiplier * JumpFallMultiplier;
	CurrentSpreadAngleMultiplier = CombinedMultiplier;
	
	// 정지 상태이며, 점프나 떨어지는 상태가 아니고, 움크리는 중이거나 푸는 중이 아님.
	return bStandingStillMultiplierAtMin && bCrouchingMultiplierAtTarget && bJumpFallMultiplierIs && bAimingMultiplierAtTarget;
}



