#pragma once

#include "Components/PawnComponent.h"
#include "BattleEquipmentManagerComponent.generated.h"

UCLASS(BlueprintType)
class UBattleEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()
public:
	UBattleEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	
};

