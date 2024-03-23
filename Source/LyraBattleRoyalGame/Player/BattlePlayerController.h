#pragma once

#include "GameFramework/PlayerController.h"
#include "ModularPlayerController.h"
#include "BattlePlayerController.generated.h"

class UBattleAbilitySystemComponent;
class ABattlePlayerState;

UCLASS()
class ABattlePlayerController : public AModularPlayerController
{
	GENERATED_BODY()
public:
	ABattlePlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void PostProcessInput(const float DeltaTime, const bool bGamePaused) override;

	ABattlePlayerState* GetBattlePlayerState() const;
	UBattleAbilitySystemComponent* GetBattleABilitySystemComponent() const;
	
	
};
