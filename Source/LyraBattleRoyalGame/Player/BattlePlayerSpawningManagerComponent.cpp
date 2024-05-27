#include "BattlePlayerSpawningManagerComponent.h"
#include "BattlePlayerStart.h"
#include "EngineUtils.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattlePlayerSpawningManagerComponent)

UBattlePlayerSpawningManagerComponent::UBattlePlayerSpawningManagerComponent(
	const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(false);
	bAutoRegister = true;
	bAutoActivate = true;
	bWantsInitializeComponent = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bAllowTickOnDedicatedServer = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UBattlePlayerSpawningManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();

	UWorld* World = GetWorld();

	for (TActorIterator<ABattlePlayerStart> It(World); It; ++It)
	{
		if (ABattlePlayerStart* PlayerStart = *It)
		{
			CachedPlayerStarts.Add(PlayerStart);
		}
	}
}

APlayerStart* UBattlePlayerSpawningManagerComponent::GetFirstRandomUnoccupiedPlayerStart(AController* Controller,
	const TArray<ABattlePlayerStart*>& FoundStartPoints) const
{
	if (Controller)
	{
		TArray<ABattlePlayerStart*> UnOccupiedStartPoints;
		TArray<ABattlePlayerStart*> OccupiedStartPoints;

		for (ABattlePlayerStart* StartPoint : FoundStartPoints)
		{
			EBattlePlayerStartLocationOccupancy State = StartPoint->GetLocationOccupancy(Controller);

			switch (State)
			{
				case EBattlePlayerStartLocationOccupancy::Empty:
					UnOccupiedStartPoints.Add(StartPoint);
					break;
				case EBattlePlayerStartLocationOccupancy::Partial:
					OccupiedStartPoints.Add(StartPoint);
					break;
			}
			
		}

		if (UnOccupiedStartPoints.Num() > 0)
		{
			return UnOccupiedStartPoints[FMath::RandRange(0, UnOccupiedStartPoints.Num() - 1)];
		}
		else if (OccupiedStartPoints.Num() > 0)
		{
			return OccupiedStartPoints[FMath::RandRange(0, OccupiedStartPoints.Num() - 1)];
		}
	}

	return nullptr;
	
}

bool UBattlePlayerSpawningManagerComponent::ControllerCanRestart(AController* Player)
{
	bool bCanRestart = true;

	return bCanRestart;
}

AActor* UBattlePlayerSpawningManagerComponent::ChoosePlayerStart(AController* Player)
{
	if (Player)
	{

		TArray<ABattlePlayerStart*> StarterPoints;
		for (auto StartIt = CachedPlayerStarts.CreateIterator(); StartIt; ++StartIt)
		{
			if (ABattlePlayerStart* Start = (*StartIt).Get())
			{
				StarterPoints.Add(Start);
			}
			else
			{
				StartIt.RemoveCurrent();
			}
		}
		
		AActor* PlayerStart = OnChoosePlayerStart(Player, StarterPoints);

		if (!PlayerStart)
		{
			PlayerStart = GetFirstRandomUnoccupiedPlayerStart(Player, StarterPoints);
		}

		if (ABattlePlayerStart* BattleStart = Cast<ABattlePlayerStart>(PlayerStart))
		{
			BattleStart->TryClaim(Player);
		}

		return PlayerStart;
	}

	return nullptr;
}

void UBattlePlayerSpawningManagerComponent::FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation)
{
	OnFinishRestartPlayer(NewPlayer, StartRotation);
	K2_OnFinishRestartPlayer(NewPlayer, StartRotation);
}

