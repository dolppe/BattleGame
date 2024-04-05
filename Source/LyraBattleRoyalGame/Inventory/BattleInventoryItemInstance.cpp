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

void UBattleInventoryItemInstance::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.AddStack(Tag,StackCount);
}

void UBattleInventoryItemInstance::RemoveStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.RemoveStack(Tag,StackCount);
}

bool UBattleInventoryItemInstance::HasStatTag(FGameplayTag Tag) const
{
	return StatTags.ContainsTag(Tag);
}

int32 UBattleInventoryItemInstance::GetStatTagStackCount(FGameplayTag Tag) const
{
	return StatTags.GetStackCount(Tag);
}
