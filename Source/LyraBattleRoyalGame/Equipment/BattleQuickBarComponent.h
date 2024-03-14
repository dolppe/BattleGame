#pragma once

#include "Components/ControllerComponent.h"
#include "BattleQuickBarComponent.generated.h"

UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UBattleQuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()
public:

	UBattleQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};

