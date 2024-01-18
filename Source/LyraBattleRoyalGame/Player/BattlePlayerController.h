#pragma once

#include "GameFramework/PlayerController.h"
#include "BattlePlayerController.generated.h"

UCLASS()
class ABattlePlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ABattlePlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
