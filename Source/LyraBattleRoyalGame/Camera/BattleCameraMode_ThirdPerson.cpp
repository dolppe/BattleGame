#include "BattleCameraMode_ThirdPerson.h"
#include "Curves/CurveVector.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleCameraMode_ThirdPerson)

UBattleCameraMode_ThirdPerson::UBattleCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UBattleCameraMode_ThirdPerson::UpdateView(float DeltaTime)
{
	// 부모와 로직이 거의 동일함.
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = GetPivotRotation();
	
	PivotRotation.Pitch = FMath::ClampAngle(PivotRotation.Pitch, ViewPitchMin, ViewPitchMax);
	
	View.Location = PivotLocation;
	View.Rotation = PivotRotation;
	View.ControlRotation = View.Rotation;
	View.FieldOfView = FieldOfView;

	// 만약 TargetOffsetCureve가 있으면, 해당 Curve의 벡터를 가져와서 Location에 적용함.
	// 내 Pitch의 Rotation값에 따라서 타겟 Offset을 가져와서 적용하는 것.
	// Curve => Curve 에셋임.
	if (TargetOffsetCurve)
	{
		const FVector TargetOffset = TargetOffsetCurve->GetVectorValue(PivotRotation.Pitch);
		View.Location = PivotLocation + PivotRotation.RotateVector(TargetOffset);
	}
}
