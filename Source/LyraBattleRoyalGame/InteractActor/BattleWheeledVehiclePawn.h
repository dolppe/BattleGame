#pragma once

#include "InteractActorInterface.h"
#include "WheeledVehiclePawn.h"
#include "GameFramework/Controller.h"
#include "BattleWheeledVehiclePawn.generated.h"

UCLASS()
class ABattleWheeledVehiclePawn : public AWheeledVehiclePawn, public IBattleInteractActorInterface
{
	GENERATED_BODY()
public:

	ABattleWheeledVehiclePawn(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	virtual void StartInteraction_Implementation(AController* InController) override;

	virtual void EndInteraction_Implementation(AController* InController) override;
	
	
};
