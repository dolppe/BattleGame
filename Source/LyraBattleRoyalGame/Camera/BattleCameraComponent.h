#pragma once
#include "Camera/CameraComponent.h"

#include "BattleCameraComponent.generated.h"

class UBattleCameraMode;
class UBattleCameraModeStack;
struct FGameplayTag;

template <class TClass> class TSubclassOf;

DECLARE_DELEGATE_RetVal(TSubclassOf<UBattleCameraMode>, FBattleCameraModeDelegate);

UCLASS()
class UBattleCameraComponent : public UCameraComponent
{
	GENERATED_BODY()
public:
	UBattleCameraComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*
	 * member Method
	 */
	static UBattleCameraComponent* FindCameraComponent(const AActor* Actor) {return (Actor ? Actor->FindComponentByClass<UBattleCameraComponent>() : nullptr); }
	AActor* GetTargetActor() const {return GetOwner();}
	void UpdateCameraModes();
	void GetBlendInfo(float& OutWeightOfTopLayer, FGameplayTag& OutTagOfTopLayer) const;
	
	/*
	 * CameraComponent Interface
	 */
	virtual void OnRegister() override;
	// 매틱 카메라가 업데이트 될때 character에서 호출
	virtual void GetCameraView(float DeltaTime, FMinimalViewInfo& DesiredView) override;

	/** 카메라의 blending 기능을 지원하는 stack */
	UPROPERTY()
	TObjectPtr<UBattleCameraModeStack> CameraModeStack;

	/** 현재 PawnData에 설정된 CameraMode를 가져오는 Delegate */
	FBattleCameraModeDelegate DetermineCameraModeDelegate;
};
