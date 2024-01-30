#pragma once

#include "BattleCameraMode.h"
#include "BattleCameraMode_ThirdPerson.generated.h"

UCLASS(Abstract, Blueprintable)
class UBattleCameraMode_ThirdPerson : public UBattleCameraMode
{
	GENERATED_BODY()
public:
	UBattleCameraMode_ThirdPerson(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	
};

