#pragma once

#include "GameFramework/PlayerStart.h"
#include "BattlePlayerStart.generated.h"

enum class EBattlePlayerStartLocationOccupancy
{
	Empty,
	Partial,
	Full
};

UCLASS(Config = Game)
class LYRABATTLEROYALGAME_API ABattlePlayerStart : public APlayerStart
{
	GENERATED_BODY()
public:

	ABattlePlayerStart(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	EBattlePlayerStartLocationOccupancy GetLocationOccupancy(AController* const ControllerPawnToFit) const;

	bool IsClaimed() const;

	bool TryClaim(AController* OccupyingController);

protected:

	void CheckUnclaimed();
	
	UPROPERTY(Transient)
	TObjectPtr<AController> ClaimingController = nullptr;

	UPROPERTY(EditDefaultsOnly, Category= "Player Start Claiming")
	float ExpirationCheckInterval = 1.0f;

	FTimerHandle ExpirationTimerHandle;
	
};

