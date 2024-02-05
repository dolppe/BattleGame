#pragma once

#include "BattleMappableConfigPair.generated.h"

class UPlayerMappableInputConfig;

USTRUCT()
struct FBattleMappableConfigPair
{
	GENERATED_BODY()
public:
	FBattleMappableConfigPair() = default;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UPlayerMappableInputConfig> Config;

	UPROPERTY(EditAnywhere)
	bool bShouldActivateAutomatically = true;
};
