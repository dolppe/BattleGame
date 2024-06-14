#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "BattleVerbMessageHelpers.generated.h"

class APlayerState;
class APlayerController;
struct FBattleVerbMessage;

UCLASS()
class LYRABATTLEROYALGAME_API UBattleVerbMessageHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category="Battle")
	static APlayerController* GetPlayerControllerFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category="Battle")
	static APlayerState* GetPlayerStateFromObject(UObject* Object);

	UFUNCTION(BlueprintCallable, Category = "Battle")
	static FBattleVerbMessage CueParametersToVerbMessage(const FGameplayCueParameters& Params);
	
};
