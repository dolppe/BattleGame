#pragma once

#include "UObject/Object.h"
#include "BattleEquipmentInstance.generated.h"


UCLASS(BlueprintType, Blueprintable)
class UBattleEquipmentInstance : public UObject
{
	GENERATED_BODY()
public:

	UBattleEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
