#include "BattleInventoryItemInstance.h"
#include "BattleInventoryItemDefinition.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleInventoryItemInstance)

UBattleInventoryItemInstance::UBattleInventoryItemInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const UBattleInventoryItemFragment* UBattleInventoryItemInstance::FindFragmentByClass(
	TSubclassOf<UBattleInventoryItemFragment> FragmentClass) const
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UBattleInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);		
	}
	return nullptr;
}
