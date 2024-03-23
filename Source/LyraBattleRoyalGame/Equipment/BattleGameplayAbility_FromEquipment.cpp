#include "BattleGameplayAbility_FromEquipment.h"

#include "BattleEquipmentInstance.h"
#include "LyraBattleRoyalGame/Inventory/BattleInventoryItemInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleGameplayAbility_FromEquipment)

UBattleGameplayAbility_FromEquipment::UBattleGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UBattleEquipmentInstance* UBattleGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		return Cast<UBattleEquipmentInstance>(Spec->SourceObject.Get());
	}
	return nullptr;
}

UBattleInventoryItemInstance* UBattleGameplayAbility_FromEquipment::GetAssociatedItem() const
{
	if (UBattleEquipmentInstance* Equipment = GetAssociatedEquipment())
	{
		return Cast<UBattleInventoryItemInstance>(Equipment->GetInstigator());
	}
	return nullptr;
	
}

