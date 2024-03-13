#pragma once

#include "GameFramework/Actor.h"
#include "BattleTaggedActor.generated.h"

UCLASS()
class ABattleTaggedActor : public AActor
{
	GENERATED_BODY()
public:
	ABattleTaggedActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
