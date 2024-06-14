#pragma once

#include "AbilitySystemInterface.h"
#include "GameFramework/GameStateBase.h"
#include "ModularGameState.h"
#include "LyraBattleRoyalGame/AbilitySystem/BattleAbilitySystemComponent.h"
#include "BattleGameState.generated.h"

class UBattleExperienceManagerComponent;
struct FBattleVerbMessage;

UCLASS()
class ABattleGameState : public AModularGameStateBase , public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABattleGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category="Battle|GameState")
	UBattleAbilitySystemComponent* GetBattleAbilitySystemComponent() const {return AbilitySystemComponent;}

	UFUNCTION(NetMulticast, Unreliable, BlueprintCallable, Category = "Battle|GameState")
	void MulticastMessageToClients(const FBattleVerbMessage Message);

	// Send a message that all clients will be guaranteed to get
	// (use only for client notifications that cannot handle being lost)
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable, Category = "Battle|GameState")
	void MulticastReliableMessageToClients(const FBattleVerbMessage Message);
	
private:
	UPROPERTY()
	TObjectPtr<UBattleExperienceManagerComponent> ExperienceManagerComponent;

	UPROPERTY(VisibleAnywhere, Category="Battle|GameState")
	TObjectPtr<UBattleAbilitySystemComponent> AbilitySystemComponent;
	
};
