#pragma once

#include "GameFramework/Character.h"
#include "BattleCharacter.generated.h"

UCLASS()
class ABattleCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	ABattleCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	
};