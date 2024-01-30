#pragma once

#include "Camera/PlayerCameraManager.h"
#include "BattlePlayerCameraManager.generated.h"

#define BATTLE_CAMERA_DEFAULT_FOV (80.0f)
#define BATTLE_CAMERA_DEFAULT_PITCH_MIN (-89.0f)
#define BATTLE_CAMERA_DEFAULT_PITCH_MAX (89.0f)

UCLASS()
class ABattlePlayerCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()
public:
	ABattlePlayerCameraManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
