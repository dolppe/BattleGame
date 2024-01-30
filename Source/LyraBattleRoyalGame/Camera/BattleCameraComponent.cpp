#include "BattleCameraComponent.h"
#include "BattleCameraMode.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleCameraComponent)

UBattleCameraComponent::UBattleCameraComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), CameraModeStack(nullptr)
{
}

void UBattleCameraComponent::UpdateCameraModes()
{
	check(CameraModeStack);

	if (DetermineCameraModeDelegate.IsBound())
	{
		if (const TSubclassOf<UBattleCameraMode> CameraMode = DetermineCameraModeDelegate.Execute())
		{
			// CameraModeStack->PushCameraMode(CameraMode);
		}
	}
	
}

void UBattleCameraComponent::OnRegister()
{
	Super::OnRegister();
	if (!CameraModeStack)
	{
		// Beginplay와 같은 초기화가 필요 없으므로 그냥 NewObject로 생성.
		CameraModeStack = NewObject<UBattleCameraModeStack>(this);
	}
}

void UBattleCameraComponent::GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView)
{
	check(CameraModeStack);

	UpdateCameraModes();
}
