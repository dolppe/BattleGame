#include "AsyncAction_ExperienceReady.h"

#include "BattleExperienceManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AsyncAction_ExperienceReady)

UAsyncAction_ExperienceReady::UAsyncAction_ExperienceReady(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UAsyncAction_ExperienceReady* UAsyncAction_ExperienceReady::WaitForExperienceReady(UObject* WorldContextObject)
{
	UAsyncAction_ExperienceReady* Action = nullptr;
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		Action = NewObject<UAsyncAction_ExperienceReady>();
		Action->WorldPtr = World;
		Action->RegisterWithGameInstance(World);
	}
	
	return Action;
}

void UAsyncAction_ExperienceReady::Activate()
{
	if (UWorld* World = WorldPtr.Get())
	{
		// 이미 World에 GAmeState가 준비되어 있으면 Step1 스킵
		if (AGameStateBase* GameState = World->GetGameState())
		{
			Step2_ListenToExperienceLoading(GameState);
		}
		else
		{
			World->GameStateSetEvent.AddUObject(this, &ThisClass::Step1_HandleGameStateSet);
		}
	}
	else
	{
		SetReadyToDestroy();
	}
	
}

void UAsyncAction_ExperienceReady::Step1_HandleGameStateSet(AGameStateBase* GameState)
{
	if (UWorld* World = WorldPtr.Get())
	{
		World->GameStateSetEvent.RemoveAll(this);
	}

	Step2_ListenToExperienceLoading(GameState);
}

void UAsyncAction_ExperienceReady::Step2_ListenToExperienceLoading(AGameStateBase* GameState)
{
	check(GameState);

	UBattleExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UBattleExperienceManagerComponent>();
	check (ExperienceManagerComponent);

	if (ExperienceManagerComponent->IsExperienceLoaded())
	{
		UWorld* World = GameState->GetWorld();
		check(World);

		World->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &UAsyncAction_ExperienceReady::Step4_BroadcastReady));
	}
	else
	{
		ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnBattleExperienceLoaded::FDelegate::CreateUObject(this, &UAsyncAction_ExperienceReady::Step3_HandleExperienceLoaded));
	}
	
}

void UAsyncAction_ExperienceReady::Step3_HandleExperienceLoaded(const UBattleExperienceDefinition* CurrentExperience)
{
	Step4_BroadcastReady();
}

void UAsyncAction_ExperienceReady::Step4_BroadcastReady()
{
	OnReady.Broadcast();
	SetReadyToDestroy();
}
