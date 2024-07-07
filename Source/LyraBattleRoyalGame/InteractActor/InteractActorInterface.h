#pragma once

#include "UObject/Interface.h"
#include "UObject/UObjectGlobals.h"
#include "InteractActorInterface.generated.h"

UINTERFACE(Blueprintable, Blueprintable)
class UBattleInteractActorInterface : public UInterface
{
	GENERATED_UINTERFACE_BODY()
};

class IBattleInteractActorInterface
{
	GENERATED_IINTERFACE_BODY()

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta=(DisplayName = "StartInteraction"))
	void StartInteraction(AController* InController); 
	
	//virtual void StartPrintWidget_Implementation(AController* InController) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta=(DisplayName = "Interact"))
	void Interact(AController* InController); 
	
	//virtual void StartInteraction_Implementation(AController* InController) = 0;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta=(DisplayName = "EndInteraction"))
	void EndInteraction(AController* InController); 
	
	//virtual void EndPrintWidget_Implementation(AController* InController) = 0;
	
	
};
