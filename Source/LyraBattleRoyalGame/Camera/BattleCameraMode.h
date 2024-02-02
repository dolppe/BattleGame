#pragma once

#include "BattleCameraMode.generated.h"

class UBattleCameraComponent;

UENUM(BlueprintType)
enum class EBattleCameraModeBlendFunction : uint8
{
	Linear,
	/**
	 * EaseIn/Out은 exponent 값에 의해 조절된다:
	 */
	EaseIn,
	EaseOut,
	EaseInOut,
	COUNT
	
};



struct FBattleCameraModeView
{
	FBattleCameraModeView();

	void Blend(const FBattleCameraModeView& Other, float OtherWeight);

	// 보간 대상의 값들
	FVector Location;
	FRotator Rotation;
	FRotator ControlRotation;
	float FieldOfView;
};



UCLASS(Abstract, NotBlueprintable)
class UBattleCameraMode : public UObject
{
	GENERATED_BODY()
public:
	UBattleCameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UBattleCameraComponent* GetBattleCameraComponent() const;
	AActor* GetTargetActor() const;
	FVector GetPivotLocation() const;
	FRotator GetPivotRotation() const;

	virtual void UpdateView(float DeltaTime);
	void UpdateBlending(float DeltaTime);
	
	void UpdateCameraMode(float DeltaTime);
	
	FBattleCameraModeView View;
	
	/** Camera Mode의 FOV */
	UPROPERTY(EditDefaultsOnly, Category="View", meta=(UIMin = "5.0", UIMax = "170", ClampMin = "5.0", ClampMax = "170.0"))
	float FieldOfView;

	/** View에 대한 Pitch [Min, Max] */
	UPROPERTY(EditDefaultsOnly, Category="View", meta=(UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMin;

	UPROPERTY(EditDefaultsOnly, Category="View", meta=(UIMin = "-89.9", UIMax = "89.9", ClampMin = "-89.9", ClampMax = "89.9"))
	float ViewPitchMax;
	
	// 얼마동안 블랜드를 진행할지
	UPROPERTY(EditDefaultsOnly, Category="Blending")
	float BlendTime;
	
	
	// [0,1] 사이로 만든 Blend값
	float BlendAlpha;
	
	/** 
	 * CameraMode의 최종 Blend 값 
	 * BlendAlpha의 선형 값을 매핑하여 최종 BlendWeight를 계산
	 */
	float BlendWeight;

	/**
	* EaseIn/Out에 사용한 Exponent
	*/	
	UPROPERTY(EditDefaultsOnly, Category="Blending")
	float BlendExponent;

	/** Blend function */
	EBattleCameraModeBlendFunction BlendFunction;
};


UCLASS()
class UBattleCameraModeStack : public UObject
{
	GENERATED_BODY()
public:
	UBattleCameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	
	UBattleCameraMode* GetCameraModeInstance(TSubclassOf<UBattleCameraMode>& CameraModeClass);
	void PushCameraMode(TSubclassOf<UBattleCameraMode>& CameraModeClass);
	void EvaluateStack(float DeltaTime, FBattleCameraModeView& OutCameraModeView);
	void UpdateStack(float DeltaTime);
	void BlendStack(FBattleCameraModeView& OutCameraModeView) const;

	
	UPROPERTY()
	TArray<TObjectPtr<UBattleCameraMode>> CameraModeInstances;

	UPROPERTY()
	TArray<TObjectPtr<UBattleCameraMode>> CameraModeStack;
	
};
