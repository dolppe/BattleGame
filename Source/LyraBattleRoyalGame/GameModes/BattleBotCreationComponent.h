#pragma once

#include "CoreMinimal.h"
#include "Components/GameStateComponent.h"

#include "BattleBotCreationComponent.generated.h"


class AAIController;
class UBattleExperienceDefinition;

UCLASS(Blueprintable, Abstract)
class UBattleBotCreationComponent : public UGameStateComponent
{
	GENERATED_BODY()

public:
	explicit UBattleBotCreationComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

private:
	void OnExperienceLoaded(const UBattleExperienceDefinition* Experience);

protected:
	UPROPERTY(EditDefaultsOnly, Category=Teams)
	int32 NumBotsToCreate = 5;

	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TSubclassOf<AAIController> BotControllerClass;

	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TArray<FString> RandomBotNames;

	TArray<FString> RemainingBotNames;

protected:
	UPROPERTY(Transient)
	TArray<TObjectPtr<AAIController>> SpawnedBotList;

#if WITH_SERVER_CODE
	
protected:
	virtual void ServerCreateBots();

	virtual void SpawnOneBot();

	FString CreateBotName(int32 PlayerIndex);

#endif
		
};
