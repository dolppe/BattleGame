#pragma once

#include "GameFramework/PlayerState.h"
#include "BattlePlayerState.generated.h"

UCLASS()
class ABattlePlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ABattlePlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
