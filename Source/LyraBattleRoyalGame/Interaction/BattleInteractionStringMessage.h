#pragma once

#include "CoreMinimal.h"

#include "BattleInteractionStringMessage.generated.h"

USTRUCT(BlueprintType)
struct FBattleInteractionStringMessage
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> Target = nullptr;

	UPROPERTY(BlueprintReadWrite)
	FString MessageString = TEXT("");
};