#pragma once

#include "Engine/EngineTypes.h"
#include "GameFramework/PlayerController.h"
#include "ModularPlayerController.generated.h"

UCLASS()
class MODULARGAMEPLAYACTORS_API AModularPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void PreInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
};


