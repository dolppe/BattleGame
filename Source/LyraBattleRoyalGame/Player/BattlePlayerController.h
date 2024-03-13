#pragma once

#include "GameFramework/PlayerController.h"
#include "ModularPlayerController.h"
#include "BattlePlayerController.generated.h"

UCLASS()
class ABattlePlayerController : public AModularPlayerController
{
	GENERATED_BODY()
public:
	ABattlePlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
