#pragma once

#include "AbilitySystemInterface.h"
#include "GameFramework/GameStateBase.h"
#include "ModularGameState.h"
#include "LyraBattleRoyalGame/AbilitySystem/BattleAbilitySystemComponent.h"
#include "BattleGameState.generated.h"

class UBattleExperienceManagerComponent;

UCLASS()
class ABattleGameState : public AModularGameStateBase , public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ABattleGameState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, Category="Battle|GameState")
	UBattleAbilitySystemComponent* GetBattleAbilitySystemComponent() const {return AbilitySystemComponent;}

private:
	UPROPERTY()
	TObjectPtr<UBattleExperienceManagerComponent> ExperienceManagerComponent;

	UPROPERTY(VisibleAnywhere, Category="Battle|GameState")
	TObjectPtr<UBattleAbilitySystemComponent> AbilitySystemComponent;
	
};
