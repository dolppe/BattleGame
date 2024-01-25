#include "BattleGameMode.h"

#include "BattleExperienceDefinition.h"
#include "BattleExperienceManagerComponent.h"
#include "BattleGameState.h"
#include "Kismet/GameplayStatics.h"
#include "LyraBattleRoyalGame/Character/BattleCharacter.h"
#include "LyraBattleRoyalGame/Character/BattlePawnExtensionComponent.h"
#include "LyraBattleRoyalGame/Player/BattlePlayerController.h"
#include "LyraBattleRoyalGame/Player/BattlePlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleGameMode)

ABattleGameMode::ABattleGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameStateClass = ABattleGameState::StaticClass();
	PlayerControllerClass = ABattlePlayerController::StaticClass();
	PlayerStateClass = ABattlePlayerState::StaticClass();
	DefaultPawnClass = ABattleCharacter::StaticClass();
	
}

void ABattleGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// 아직 GameInstance를 통해 초기화 작업이 진행중인 상태
	// 현 프레임에는 Experience 처리가 불가능함 => 한 프레임 뒤에 처리하도록 걸어두는 것.
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

void ABattleGameMode::InitGameState()
{
	Super::InitGameState();

	// Experience 비동기 로딩을 위한 델리게이트
	UBattleExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UBattleExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnBattleExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void ABattleGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (IsExperienceLoaded())
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

APawn* ABattleGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer,
	const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;
	SpawnInfo.bDeferConstruction = true;

	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform,SpawnInfo))
		{
			if (UBattlePawnExtensionComponent* PawnExtComp = UBattlePawnExtensionComponent::FindPawnExtensionComponent(SpawnedPawn))
			{
				if (const UBattlePawnData* PawnData = GetPawnDataForController(NewPlayer))
				{
					PawnExtComp->SetPawnData(PawnData);
				}
			}
			SpawnedPawn->FinishSpawning(SpawnTransform);
			return SpawnedPawn;
		}
	}
	
	return nullptr;	
}

UClass* ABattleGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (const UBattlePawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ABattleGameMode::HandleMatchAssignmentIfNotExpectingOne()
{
	// Experience에 대해 PrimaryAssetId를 생성하여, OnMatchAssignmentGiven으로 넘겨줌.
	FPrimaryAssetId ExperienceId;

	UWorld* World = GetWorld();

	// 만들어둔 것을 Default하게 설정해서 PrimaryAssetId로 생성함
	// 5.1 Ver 기준으로는 Config나 다른 곳에서 가져오는 것이 아니라 그냥 바로 코드상 입력되어 있음.
	if (!ExperienceId.IsValid())
	{
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("BattleExperienceDefinition"), FName("BP_BattleDefaultExperience"));
	}

	OnMatchAssignmentGiven(ExperienceId);
	
}

void ABattleGameMode::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId)
{
	// ExperienceManagerComponent를 활용하여 Experience를 로딩하기 위함.
	check(ExperienceId.IsValid());

	UBattleExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UBattleExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	ExperienceManagerComponent->ServerSetCurrentExperience(ExperienceId);
	
}

bool ABattleGameMode::IsExperienceLoaded() const
{
	check(GameState);
	UBattleExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UBattleExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	return ExperienceManagerComponent->IsExperienceLoaded();
}

const UBattlePawnData* ABattleGameMode::GetPawnDataForController(const AController* InController) const
{
	// 이미 PlayerState에 PawnData가 존재하는 경우 해당 PawnData 리턴
	if (InController)
	{
		if (const ABattlePlayerState* BattlePS = InController->GetPlayerState<ABattlePlayerState>())
		{
			if (const UBattlePawnData* PawnData = BattlePS->GetPawnData<UBattlePawnData>())
			{
				return PawnData;
			}
		}
	}
	
	check(GameState);
	UBattleExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UBattleExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	// Experience가 로딩됐으면 DefaultPawnData를 가져와서 리턴
	if (ExperienceManagerComponent->IsExperienceLoaded())
	{
		const UBattleExperienceDefinition* Experience = ExperienceManagerComponent->GetCurrentExperienceChecked();

		if (Experience->DefaultPawnData)
		{
			return Experience->DefaultPawnData;
		}
	}
	return nullptr;
}

void ABattleGameMode::OnExperienceLoaded(const UBattleExperienceDefinition* CurrentExperience)
{
	// PC를 순회
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator();Iterator;++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);

		// PC가 아직 Pawn에 Possess하지 않았으면, RestartPlayer를 통해 Pawn을 다시 Spawn함.
		if (PC && PC->GetPawn() == nullptr)
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
	
}
