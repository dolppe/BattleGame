#include "BattlePlayerController.h"

#include "BattlePlayerState.h"
#include "LyraBattleRoyalGame/AbilitySystem/BattleAbilitySystemComponent.h"
#include "LyraBattleRoyalGame/Camera/BattlePlayerCameraManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(BattlePlayerController)

ABattlePlayerController::ABattlePlayerController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PlayerCameraManagerClass = ABattlePlayerCameraManager::StaticClass();
}

void ABattlePlayerController::PostProcessInput(const float DeltaTime, const bool bGamePaused)
{
	if (UBattleAbilitySystemComponent* BattleASC = GetBattleABilitySystemComponent())
	{
		BattleASC->ProcessAbilityInput(DeltaTime, bGamePaused);
	}
	
	Super::PostProcessInput(DeltaTime, bGamePaused);

	
}

ABattlePlayerState* ABattlePlayerController::GetBattlePlayerState() const
{
	return CastChecked<ABattlePlayerState>(PlayerState, ECastCheckedType::NullAllowed);
}

UBattleAbilitySystemComponent* ABattlePlayerController::GetBattleABilitySystemComponent() const
{
	const ABattlePlayerState* BattlePS = GetBattlePlayerState();
	return (BattlePS ? BattlePS->GetBattleAbilitySystemComponent() : nullptr);
}
