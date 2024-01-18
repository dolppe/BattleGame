#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleGameMode.generated.h"

UCLASS()
class ABattleGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ABattleGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
