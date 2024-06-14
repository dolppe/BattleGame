#include "BattlePlayerBotController.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "AbilitySystemInterface.h"
#include "BattlePlayerState.h"
#include "LyraBattleRoyalGame/GameModes/BattleGameMode.h"
#include "Perception/AIPerceptionComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattlePlayerBotController)

ABattlePlayerBotController::ABattlePlayerBotController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ABattlePlayerBotController::ServerRestartController()
{
	if (GetNetMode() == NM_Client)
	{
		return;
	}

	ensure((GetPawn() == nullptr) && IsInState(NAME_Inactive));

	if (IsInState(NAME_Inactive) || (IsInState(NAME_Spectating)))
	{
		ABattleGameMode* const GameMode = GetWorld()->GetAuthGameMode<ABattleGameMode>();

		if ((GameMode == nullptr) || !GameMode->ControllerCanRestart(this))
		{
			return;
		}

		if (GetPawn() != nullptr)
		{
			UnPossess();
		}

		ResetIgnoreInputFlags();

		GameMode->RestartPlayer(this);
	}
	
}

void ABattlePlayerBotController::OnUnPossess()
{
	if (APawn* PawnBeingUnpossessed = GetPawn())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(PlayerState))
		{
			if (ASC->GetAvatarActor() == PawnBeingUnpossessed)
			{
				ASC->SetAvatarActor(nullptr);
			}
		}
	}
	
	Super::OnUnPossess();
}

void ABattlePlayerBotController::UpdateTeamAttitude(UAIPerceptionComponent* AIPerception)
{
	if (AIPerception)
	{
		AIPerception->RequestStimuliListenerUpdate();
	}
}
