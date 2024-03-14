#include "BattleEquipmentManagerComponent.h"
#include "BattleEquipmentDefinition.h"
#include "BattleEquipmentInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleEquipmentManagerComponent)

UBattleEquipmentManagerComponent::UBattleEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EquipmentList(this)
{
}
