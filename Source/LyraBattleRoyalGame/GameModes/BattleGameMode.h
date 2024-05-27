#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BattleGameMode.generated.h"

class UBattleExperienceDefinition;
class UBattlePawnData;

UCLASS()
class ABattleGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	ABattleGameMode(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	/*
	 * AGameModeBase Interface
	 */ 
	
	// 초반부에 호출, 게임 세션 초기화 등 작업 진행.
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void InitGameState() override;

	/** HandleStartingNewPlayer */
	// Experience가 없는 상태에는 PawnData가 없는데, 그 상황에서 Pawn을 Spawn하고 PC가 Possess하는 것을 막음.
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	/** SpawnDefaultPawnAtTransform */
	// PawnData를 이용해서 실제로 Spawn을 진행할 수 있도록 재정의해서 구현
	virtual APawn* SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform) override;

	/** GetDefaultPawnClassForController */
	// PC의 PawnData 혹은 Experience의 DefaultPawnData를 리턴해줌.
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual bool PlayerCanRestart_Implementation(APlayerController* Player) override;
	
	/*
	 * Member Methods
	 */
	
	// 다음 프레임이 시작될 때 진짜 초기화를 진행하기 위한 함수
	void HandleMatchAssignmentIfNotExpectingOne();
	void OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId);
	bool IsExperienceLoaded() const;
	const UBattlePawnData* GetPawnDataForController(const AController* InController) const;
	void OnExperienceLoaded(const UBattleExperienceDefinition* CurrentExperience);

	virtual bool ControllerCanRestart(AController* Controller);

	
};
