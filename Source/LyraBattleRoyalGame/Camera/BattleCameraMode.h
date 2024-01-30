#pragma once

#include "BattleCameraMode.generated.h"

UCLASS(Abstract, NotBlueprintable)
class UBattleCameraMode : public UObject
{
	GENERATED_BODY()
public:
	UBattleCameraMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};


UCLASS()
class UBattleCameraModeStack : public UObject
{
	GENERATED_BODY()
public:
	UBattleCameraModeStack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY()
	TArray<TObjectPtr<UBattleCameraMode>> CameraModeInstances;

	UPROPERTY()
	TArray<TObjectPtr<UBattleCameraMode>> CameraModeStack;
	
};
