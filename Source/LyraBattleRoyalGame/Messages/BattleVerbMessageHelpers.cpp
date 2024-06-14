#include "BattleVerbMessageHelpers.h"

#include "BattleVerbMessage.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleVerbMessageHelpers)

APlayerController* UBattleVerbMessageHelpers::GetPlayerControllerFromObject(UObject* Object)
{
	if (APlayerController* PC = Cast<APlayerController>(Object))
	{
		return PC;
	}

	if (APlayerState* TargetPS = Cast<APlayerState>(Object))
	{
		return TargetPS->GetPlayerController();
	}

	if (APawn* TargetPawn = Cast<APawn>(Object))
	{
		return Cast<APlayerController>(TargetPawn->GetController());
	}

	return nullptr;
}

APlayerState* UBattleVerbMessageHelpers::GetPlayerStateFromObject(UObject* Object)
{
	if (APlayerController* PC = Cast<APlayerController>(Object))
	{
		return PC->PlayerState;
	}

	if (APlayerState* TargetPS = Cast<APlayerState>(Object))
	{
		return TargetPS;
	}

	if (APawn* TargetPawn = Cast<APawn>(Object))
	{
		if (APlayerState* TargetPS = TargetPawn->GetPlayerState())
		{
			return TargetPS;
		}
	}

	return nullptr;
}

FBattleVerbMessage UBattleVerbMessageHelpers::CueParametersToVerbMessage(const FGameplayCueParameters& Params)
{
	FBattleVerbMessage Result;

	Result.Verb = Params.OriginalTag;
	Result.Instigator = Params.Instigator.Get();
	Result.Target = Params.EffectCauser.Get();
	Result.InstigatorTags = Params.AggregatedSourceTags;
	Result.TargetTags = Params.AggregatedTargetTags;
	Result.Magnitude = Params.RawMagnitude;

	return Result;
}
