#pragma once

#include "BattleCameraMode.h"
#include "BattleCameraMode_ThirdPerson.generated.h"

class UCurveVector;

UCLASS(Abstract, Blueprintable)
class UBattleCameraMode_ThirdPerson : public UBattleCameraMode
{
	GENERATED_BODY()
public:
	UBattleCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void UpdateView(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category= "Thire Person")
	TObjectPtr<const UCurveVector> TargetOffsetCurve; 
	
	
};

