#include "BattleEquipmentDefinition.h"

#include "BattleEquipmentInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleEquipmentDefinition)

UBattleEquipmentDefinition::UBattleEquipmentDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InstanceType = UBattleEquipmentInstance::StaticClass();
}
