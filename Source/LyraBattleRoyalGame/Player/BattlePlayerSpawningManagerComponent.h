#pragma once

#include "Components/GameStateComponent.h"
#include "BattlePlayerSpawningManagerComponent.generated.h"

class AController;
class APlayerController;
class APlayerState;
class APlayerStart;
class ABattlePlayerStart;
class AActor;

UCLASS()
class LYRABATTLEROYALGAME_API UBattlePlayerSpawningManagerComponent : public UGameStateComponent
{
	GENERATED_BODY()
public:
	UBattlePlayerSpawningManagerComponent(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeComponent() override;

protected:

	APlayerStart* GetFirstRandomUnoccupiedPlayerStart(AController* Controller, const TArray<ABattlePlayerStart*>& FoundStartPoints) const;

	virtual AActor* OnChoosePlayerStart(AController* Player, TArray<ABattlePlayerStart*>& PlayerStarts) { return nullptr; }
	virtual void OnFinishRestartPlayer(AController* Player, const FRotator& StartRotation) { }

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName=OnFinishRestartPlayer))
	void K2_OnFinishRestartPlayer(AController* Player, const FRotator& StartRotation);

private:

	friend class ABattleGameMode;
	bool ControllerCanRestart(AController* Player);
	AActor* ChoosePlayerStart(AController* Player);
	void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation);

	UPROPERTY(Transient)
	TArray<TWeakObjectPtr<ABattlePlayerStart>> CachedPlayerStarts;

};

