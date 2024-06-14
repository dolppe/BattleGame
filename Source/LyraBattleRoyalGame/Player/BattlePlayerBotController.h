#pragma once

#include "ModularAIController.h"
#include "BattlePlayerBotController.generated.h"

class UAIPerceptionComponent;

UCLASS(Blueprintable)
class ABattlePlayerBotController : public AModularAIController
{
	GENERATED_BODY()
public:
	ABattlePlayerBotController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void ServerRestartController();
	
	virtual void OnUnPossess() override;
	
	UFUNCTION(BlueprintCallable, Category = "Battle AI Player Controller")
    void UpdateTeamAttitude(UAIPerceptionComponent* AIPerception);
	
	
};

