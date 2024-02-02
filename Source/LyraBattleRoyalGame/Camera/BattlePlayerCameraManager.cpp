#include "BattlePlayerCameraManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(BattlePlayerCameraManager)

ABattlePlayerCameraManager::ABattlePlayerCameraManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	DefaultFOV = BATTLE_CAMERA_DEFAULT_FOV;
	ViewPitchMin = BATTLE_CAMERA_DEFAULT_PITCH_MIN;
	ViewPitchMax = BATTLE_CAMERA_DEFAULT_PITCH_MAX;
}
