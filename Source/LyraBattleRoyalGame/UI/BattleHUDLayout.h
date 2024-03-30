#pragma once

#include "BattleActivatableWidget.h"
#include "BattleHUDLayout.generated.h"

/*
 *	PrimaryGameLayout의 Layer에 연동할 HUD Layout
 */

UCLASS(Abstract, BlueprintType, meta=(DisplayName="Battle HUD Layout", Category="Battle|HUD"))
class UBattleHUDLayout : public UBattleActivatableWidget
{
	GENERATED_BODY()
public:
	UBattleHUDLayout(const FObjectInitializer& ObjectInitializer);
	
};

