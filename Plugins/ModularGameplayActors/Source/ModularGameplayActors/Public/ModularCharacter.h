#pragma once

#include "Engine/EngineTypes.h"
#include "GameFramework/Character.h"
#include "ModularCharacter.generated.h"

UCLASS()
class MODULARGAMEPLAYACTORS_API AModularCharacter : public ACharacter
{
	GENERATED_BODY()
public:

	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
};
