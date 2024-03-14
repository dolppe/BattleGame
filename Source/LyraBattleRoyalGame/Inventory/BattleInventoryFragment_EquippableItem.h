#pragma once

#include "BattleInventoryItemDefinition.h"
#include "Templates/SubclassOf.h"
#include "BattleInventoryFragment_EquippableItem.generated.h"

class UBattleEquipmentDefinition;

UCLASS()
class UBattleInventoryFragment_EquippableItem : public UBattleInventoryItemFragment
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category=Battle)
	TSubclassOf<UBattleEquipmentDefinition> EquipmentDefinition;
	
};


