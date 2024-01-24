#include "BattleGameState.h"

#include "BattleExperienceManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleGameState)

ABattleGameState::ABattleGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ExperienceManagerComponent = CreateDefaultSubobject<UBattleExperienceManagerComponent>(TEXT("ExperneceManagerComponent"));
}
