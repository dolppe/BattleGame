#pragma once

#include "ModularAIController.h"
#include "BattlePlayerBotController.generated.h"

UCLASS()
class ABattlePlayerBotController : public AModularAIController
{
	GENERATED_BODY()
public:
	ABattlePlayerBotController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void ServerRestartController();
	
	virtual void OnUnPossess() override;
	
};

