#pragma once

#include "GameplayCueManager.h"
#include "BattleGameplayCueManager.generated.h"

UCLASS()
class UBattleGameplayCueManager : public UGameplayCueManager
{
	GENERATED_BODY()
public:
	static UBattleGameplayCueManager* Get();

	UBattleGameplayCueManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	void RefreshGameplayCuePrimaryAsset();
};
