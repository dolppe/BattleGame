#include "BattleGamePhaseSubsystem.h"

#include "BattleGamePhaseAbility.h"
#include "GameFramework/GameStateBase.h"
#include "LyraBattleRoyalGame/BattleLogChannels.h"

#include "LyraBattleRoyalGame/AbilitySystem/BattleAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleGamePhaseSubsystem)

UBattleGamePhaseSubsystem::UBattleGamePhaseSubsystem()
{
}

void UBattleGamePhaseSubsystem::StartPhase(TSubclassOf<UBattleGamePhaseAbility> PhaseAbility, FBattleGamePhaseDelegate PhaseEndedCallback)
{
	UWorld* World = GetWorld();
	UBattleAbilitySystemComponent* GameState_ASC = World->GetGameState()->FindComponentByClass<UBattleAbilitySystemComponent>();
	if (ensure(GameState_ASC))
	{
		FGameplayAbilitySpec PhaseSpec(PhaseAbility, 1, 0, this);
		FGameplayAbilitySpecHandle SpecHandle = GameState_ASC->GiveAbilityAndActivateOnce(PhaseSpec);
		FGameplayAbilitySpec* FoundSpec = GameState_ASC->FindAbilitySpecFromHandle((SpecHandle));

		if (FoundSpec && FoundSpec->IsActive())
		{
			FBattleGamePhaseEntry& Entry = ActivePhaseMap.FindOrAdd(SpecHandle);
			Entry.PhaseEndedCallback = PhaseEndedCallback;
		}
		else
		{
			PhaseEndedCallback.ExecuteIfBound(nullptr);
		}
	}
}

void UBattleGamePhaseSubsystem::WhenPhaseStartsOrIsActive(FGameplayTag PhaseTag, EPhaseTagMatchType MatchType,
	const FBattleGamePhaseTagDelegate& WhenPhaseActive)
{
	FPhaseObserver Observer;
	Observer.PhaseTag = PhaseTag;
	Observer.MatchType = MatchType;
	Observer.PhaseCallback = WhenPhaseActive;
	PhaseStartObservers.Add(Observer);
	
	if (IsPhaseActive(PhaseTag))
	{
		WhenPhaseActive.ExecuteIfBound(PhaseTag);
	}
}

void UBattleGamePhaseSubsystem::WhenPhaseEnds(FGameplayTag PhaseTag, EPhaseTagMatchType MatchType,
	const FBattleGamePhaseTagDelegate& WhenPhaseEnd)
{
	FPhaseObserver Observer;
	Observer.PhaseTag = PhaseTag;
	Observer.MatchType = MatchType;
	Observer.PhaseCallback = WhenPhaseEnd;
	PhaseEndObservers.Add(Observer);
}

bool UBattleGamePhaseSubsystem::IsPhaseActive(const FGameplayTag& PhaseTag) const
{
	for (const auto& KVP : ActivePhaseMap)
	{
		const FBattleGamePhaseEntry& PhaseEntry = KVP.Value;
		if (PhaseEntry.PhaseTag.MatchesTag(PhaseTag))
		{
			return true;
		}
	}

	return false;
}

bool UBattleGamePhaseSubsystem::DoesSupportWorldType(const EWorldType::Type WorldType) const
{
	return WorldType == EWorldType::Game || WorldType == EWorldType::PIE;
}

void UBattleGamePhaseSubsystem::K2_StartPhase(TSubclassOf<UBattleGamePhaseAbility> Phase,
	const FBattleGamePhaseDynamicDelegate& PhaseEnded)
{
	const FBattleGamePhaseDelegate EndedDelegate = FBattleGamePhaseDelegate::CreateWeakLambda(const_cast<UObject*>(PhaseEnded.GetUObject()),[PhaseEnded](const UBattleGamePhaseAbility* PhaseAbility)
	{
		PhaseEnded.ExecuteIfBound(PhaseAbility);
	});

	StartPhase(Phase, EndedDelegate);
}

void UBattleGamePhaseSubsystem::OnBeginPhase(const UBattleGamePhaseAbility* PhaseAbility,
                                             const FGameplayAbilitySpecHandle PhaseAbilityHandle)
{
	const FGameplayTag InComingPhaseTag = PhaseAbility->GetGamePhaseTag();

	UE_LOG(LogBattle, Log, TEXT("Phase Start: %s"), *InComingPhaseTag.ToString());

	const UWorld* World = GetWorld();
	UBattleAbilitySystemComponent* GameState_ASC = World->GetGameState()->FindComponentByClass<UBattleAbilitySystemComponent>();
	if (ensure(GameState_ASC))
	{
		TArray<FGameplayAbilitySpec*> ActivePhases;
		for (const auto& KVP : ActivePhaseMap)
		{
			const FGameplayAbilitySpecHandle ActiveAbilityHandle = KVP.Key;
			if (FGameplayAbilitySpec* Spec = GameState_ASC->FindAbilitySpecFromHandle(ActiveAbilityHandle))
			{
				ActivePhases.Add(Spec);
			}
		}

		for (const FGameplayAbilitySpec* ActivePhase : ActivePhases)
		{
			const UBattleGamePhaseAbility* ActivePhaseAbility = CastChecked<UBattleGamePhaseAbility>(ActivePhase->Ability);
			const FGameplayTag ActivePhaseTag = ActivePhaseAbility->GetGamePhaseTag();

			// ImcomingPhaseTag가 ActivePhaseTag를 Has 하고 있어야 True임.
			// 즉 없는 경우임.
			// 들어온 태그가 활성화된 태그의 자식 태그가 아니면, 해당 Phase를 종료시킴.
			if (!InComingPhaseTag.MatchesTag(ActivePhaseTag))
			{
				UE_LOG(LogBattle, Log, TEXT("Ending Phase: %s"), *ActivePhaseTag.ToString());

				FGameplayAbilitySpecHandle HandleToEnd = ActivePhase->Handle;
				GameState_ASC->CancelAbilityByFunc([HandleToEnd](const UBattleGameplayAbility* BattleAbility, FGameplayAbilitySpecHandle Handle)
				{
					return Handle == HandleToEnd;
				}, true);
			}
		}

		FBattleGamePhaseEntry& Entry = ActivePhaseMap.FindOrAdd(PhaseAbilityHandle);
		Entry.PhaseTag = InComingPhaseTag;

		for (const FPhaseObserver& Observer : PhaseStartObservers)
		{
			if (Observer.IsMatch(InComingPhaseTag))
			{
				Observer.PhaseCallback.ExecuteIfBound(InComingPhaseTag);
			}
		}
	}
	
	
}

void UBattleGamePhaseSubsystem::OnEndPhase(const UBattleGamePhaseAbility* PhaseAbility,
	const FGameplayAbilitySpecHandle PhaseAbilityHandle)
{
	const FGameplayTag EndedPhaseTag = PhaseAbility->GetGamePhaseTag();
	UE_LOG(LogBattle, Log, TEXT("Ended Phase %s"), *EndedPhaseTag.ToString());

	const FBattleGamePhaseEntry& Entry = ActivePhaseMap.FindChecked(PhaseAbilityHandle);
	Entry.PhaseEndedCallback.ExecuteIfBound(PhaseAbility);

	ActivePhaseMap.Remove(PhaseAbilityHandle);

	for (const FPhaseObserver& Observer : PhaseEndObservers)
	{
		if (Observer.IsMatch(EndedPhaseTag))
		{
			Observer.PhaseCallback.ExecuteIfBound(EndedPhaseTag);
		}
	}
}

bool UBattleGamePhaseSubsystem::FPhaseObserver::IsMatch(const FGameplayTag& ComparePhaseTag) const
{
	switch(MatchType)
	{
	case EPhaseTagMatchType::ExactMatch:
		return ComparePhaseTag == PhaseTag;
	case EPhaseTagMatchType::PartialMatch:
		return ComparePhaseTag.MatchesTag(PhaseTag);
	}

	return false;
}
