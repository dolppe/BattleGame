#include "BattleGameMode.h"

#include "BattleGameState.h"
#include "LyraBattleRoyalGame/Character/BattleCharacter.h"
#include "LyraBattleRoyalGame/Player/BattlePlayerController.h"
#include "LyraBattleRoyalGame/Player/BattlePlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleGameMode)

ABattleGameMode::ABattleGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = ABattleGameState::StaticClass();
	PlayerControllerClass = ABattlePlayerController::StaticClass();
	PlayerStateClass = ABattlePlayerState::StaticClass();
	DefaultPawnClass = ABattleCharacter::StaticClass();
	
}
