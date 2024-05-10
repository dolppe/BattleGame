#include "BattleReticleWidgetBase.h"

#include "LyraBattleRoyalGame/Inventory/BattleInventoryItemInstance.h"
#include "LyraBattleRoyalGame/Weapons/BattleRangedWeaponInstance.h"
#include "LyraBattleRoyalGame/Weapons/BattleWeaponInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleReticleWidgetBase)

UBattleReticleWidgetBase::UBattleReticleWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UBattleReticleWidgetBase::InitializeFromWeapon(UBattleWeaponInstance* InWeapon)
{
	WeaponInstance = InWeapon;
	InventoryInstance = nullptr;

	if (WeaponInstance)
	{
		InventoryInstance = Cast<UBattleInventoryItemInstance>(WeaponInstance->GetInstigator());
	}
	OnWeaponInitialized();
}

float UBattleReticleWidgetBase::ComputeSpreadAngle() const
{
	if (const UBattleRangedWeaponInstance* RangedWeapon = Cast<const UBattleRangedWeaponInstance>(WeaponInstance))
	{
		const float BaseSpreadAngle = RangedWeapon->GetCalculatedSpreadAngle();
		const float SpreadAngleMultiplier = RangedWeapon->GetCalculatedSpreadAngleMultiplier();
		const float ActualSpreadAngle = BaseSpreadAngle * SpreadAngleMultiplier;

		return ActualSpreadAngle;
	}
	else
	{
		return 0.0f;
	}
}

float UBattleReticleWidgetBase::ComputeMaxScreenspaceSpreadRadius() const
{
	const float LongShotDistance = 10000.f;

	APlayerController* PC = GetOwningPlayer();
	if (PC && PC->PlayerCameraManager)
	{
		const float SpreadRadiusRads = FMath::DegreesToRadians(ComputeSpreadAngle() * 0.5f);
		const float SpreadRadiusAtDistance = FMath::Tan(SpreadRadiusRads) * LongShotDistance;

		FVector CamPos;
		FRotator CamRot;
		PC->PlayerCameraManager->GetCameraViewPoint(CamPos, CamRot);

		FVector CamForwDir = CamRot.RotateVector(FVector::ForwardVector);
		FVector CamUpDir = CamRot.RotateVector(FVector::UpVector);

		// 카메라의 위치에서 Foward한 Vector와 Spread 원뿔 절반의 대변만큼의 Up Vector를 더함
		// 이게 확산에 대한 벡터가 됨.
		FVector OffsetTargetAtDistance = CamPos + (CamForwDir*LongShotDistance) + (CamUpDir * SpreadRadiusAtDistance);

		// 위에서 구한 Vector를 Screen에 투영시킴.
		FVector2D OffsetTargetInScreenspace;

		if (PC->ProjectWorldLocationToScreen(OffsetTargetAtDistance, OffsetTargetInScreenspace, true))
		{
			int32 ViewportSizeX(0), ViewportSizeY(0);
			PC->GetViewportSize(ViewportSizeX, ViewportSizeY);

			// 스크린의 센터값
			const FVector2D ScreenSpaceCenter(FVector::FReal(ViewportSizeX) * 0.5f, FVector::FReal(ViewportSizeY) * 0.5f);
			
			return (OffsetTargetInScreenspace - ScreenSpaceCenter).Length();
		}
	}
	
	return 0.0f;	
}

bool UBattleReticleWidgetBase::HasFirstShotAccuracy() const
{
	if (const UBattleRangedWeaponInstance* RangedWeapon = Cast<const UBattleRangedWeaponInstance>(WeaponInstance))
	{
		return RangedWeapon->HasFirstShotAccuracy();
	}
	else
	{
		return false;
	}
	
}
