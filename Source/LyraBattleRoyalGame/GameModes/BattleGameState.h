#pragma once

#include "GameFramework/GameStateBase.h"
#include "BattleGameState.generated.h"

UCLASS()
class ABattleGameState : public AGameStateBase 
{
	GENERATED_BODY()

public:
	ABattleGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	
};
