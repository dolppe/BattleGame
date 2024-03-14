#include "BattleInventoryItemDefinition.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleInventoryItemDefinition)

UBattleInventoryItemDefinition::UBattleInventoryItemDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const UBattleInventoryItemFragment* UBattleInventoryItemDefinition::FindFragmentByClass(
	TSubclassOf<UBattleInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass)
	{
		for (UBattleInventoryItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}
	return nullptr;
	
}
