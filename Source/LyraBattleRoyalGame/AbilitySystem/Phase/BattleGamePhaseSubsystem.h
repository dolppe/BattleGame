#pragma once

#include "Subsystems/WorldSubsystem.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpec.h"
#include "BattleGamePhaseSubsystem.generated.h"


DECLARE_DYNAMIC_DELEGATE_OneParam(FBattleGamePhaseDynamicDelegate, const UBattleGamePhaseAbility* ,Phase);
DECLARE_DELEGATE_OneParam(FBattleGamePhaseDelegate, const UBattleGamePhaseAbility* Phase);

DECLARE_DYNAMIC_DELEGATE_OneParam(FBattleGamePhaseTagDynamicDelegate, const FGameplayTag& ,Phase);
DECLARE_DELEGATE_OneParam(FBattleGamePhaseTagDelegate, const FGameplayTag& PhaseTag);

UENUM()
enum class EPhaseTagMatchType : uint8
{
	ExactMatch,
	PartialMatch
};

UCLASS()
class UBattleGamePhaseSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	UBattleGamePhaseSubsystem();
	
	void StartPhase(TSubclassOf<UBattleGamePhaseAbility> PhaseAbility, FBattleGamePhaseDelegate PhaseEndedCallback = FBattleGamePhaseDelegate());

	void WhenPhaseStartsOrIsActive(FGameplayTag PhaseTag, EPhaseTagMatchType MatchType, const FBattleGamePhaseTagDelegate& WhenPhaseActive);
	void WhenPhaseEnds(FGameplayTag PhaseTag, EPhaseTagMatchType MatchType, const FBattleGamePhaseTagDelegate& WhenPhaseEnd);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, BlueprintPure = false, meta = (AutoCreateRefTerm = "PhaseTag"))
	bool IsPhaseActive(const FGameplayTag& PhaseTag) const;
	
protected:

	virtual bool DoesSupportWorldType(const EWorldType::Type WorldType) const override;	
	
	void OnBeginPhase(const UBattleGamePhaseAbility* PhaseAbility, const FGameplayAbilitySpecHandle PhaseAbilityHandle);
	void OnEndPhase(const UBattleGamePhaseAbility* PhaseAbility, const FGameplayAbilitySpecHandle PhaseAbilityHandle);

private:

	friend class UBattleGamePhaseAbility;

	struct FBattleGamePhaseEntry
	{
	public:
		FGameplayTag PhaseTag;
		FBattleGamePhaseDelegate PhaseEndedCallback;
	};

	TMap<FGameplayAbilitySpecHandle, FBattleGamePhaseEntry> ActivePhaseMap;

	struct FPhaseObserver
	{
	public:
		bool IsMatch(const FGameplayTag& ComparePhaseTag) const;

		FGameplayTag PhaseTag;
		EPhaseTagMatchType MatchType = EPhaseTagMatchType::ExactMatch;
		FBattleGamePhaseTagDelegate PhaseCallback;
	};

	TArray<FPhaseObserver> PhaseStartObservers;
	TArray<FPhaseObserver> PhaseEndObservers;	
	
};
