#include "BattleBotCreationComponent.h"

#include "AIController.h"
#include "BattleExperienceManagerComponent.h"
#include "BattleGameMode.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "LyraBattleRoyalGame/Character/BattlePawnExtensionComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleBotCreationComponent)

UBattleBotCreationComponent::UBattleBotCreationComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UBattleBotCreationComponent::BeginPlay()
{
	Super::BeginPlay();

	AGameStateBase* GameState = GetGameStateChecked<AGameStateBase>();
	UBattleExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<UBattleExperienceManagerComponent>();
	check(ExperienceComponent);
	ExperienceComponent->CallOrRegister_OnExperienceLoaded(FOnBattleExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void UBattleBotCreationComponent::OnExperienceLoaded(const UBattleExperienceDefinition* Experience)
{
#if WITH_SERVER_CODE
	if (HasAuthority())
	{
		ServerCreateBots();
	}
#endif
}

void UBattleBotCreationComponent::ServerCreateBots()
{
	if (BotControllerClass == nullptr)
	{
		return;
	}

	RemainingBotNames = RandomBotNames;

	int32 EffectiveBotCount = NumBotsToCreate;

	if (AGameModeBase* GameModeBase = GetGameMode<AGameModeBase>())
	{
		EffectiveBotCount = UGameplayStatics::GetIntOption(GameModeBase->OptionsString, TEXT("NumBots"), EffectiveBotCount);
	}

	for (int32 Count = 0; Count <EffectiveBotCount; ++Count)
	{
		SpawnOneBot();
	}
	
}

void UBattleBotCreationComponent::SpawnOneBot()
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnInfo.OverrideLevel = GetComponentLevel();
	SpawnInfo.ObjectFlags |= RF_Transient;
	AAIController* NewController = GetWorld()->SpawnActor<AAIController>(BotControllerClass, FVector::ZeroVector, FRotator::ZeroRotator,SpawnInfo);

	if (NewController != nullptr)
	{
		ABattleGameMode* GameMode = GetGameMode<ABattleGameMode>();
		check(GameMode)

		if (NewController->PlayerState != nullptr)
		{
			NewController->PlayerState->SetPlayerName(CreateBotName(NewController->PlayerState->GetPlayerId()));
		}

		GameMode->DispatchPostLogin(NewController);
		GameMode->RestartPlayer(NewController);

		if (NewController->GetPawn() != nullptr)
		{
			if (UBattlePawnExtensionComponent* PawnExtComp = NewController->GetPawn()->FindComponentByClass<UBattlePawnExtensionComponent>())
			{
				PawnExtComp->CheckDefaultInitialization();
			}
		}

		SpawnedBotList.Add(NewController);
	}
}

FString UBattleBotCreationComponent::CreateBotName(int32 PlayerIndex)
{
	FString Result;
	if (RemainingBotNames.Num() >0)
	{
		const int32 NameIndex = FMath::RandRange(0, RemainingBotNames.Num()-1);
		Result = RemainingBotNames[NameIndex];
		RemainingBotNames.RemoveAtSwap(NameIndex);
	}
	else
	{
		PlayerIndex = FMath::RandRange(260, 260+100);
		Result = FString::Printf(TEXT("Tinplate %d"), PlayerIndex);
	}
	return Result;
}
