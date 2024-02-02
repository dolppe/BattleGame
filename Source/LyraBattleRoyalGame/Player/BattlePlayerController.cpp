#include "BattlePlayerController.h"
#include "LyraBattleRoyalGame/Camera/BattlePlayerCameraManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(BattlePlayerController)

ABattlePlayerController::ABattlePlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerCameraManagerClass = ABattlePlayerCameraManager::StaticClass();
}
