#include "BattleInventoryFragment_SetStats.h"
#include "BattleInventoryItemInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleInventoryFragment_SetStats)

void UBattleInventoryFragment_SetStats::OnInstanceCreated(UBattleInventoryItemInstance* Instance) const
{
	for (const auto& InitialItemStat : InitialItemStats)
	{
		Instance->AddStatTagStack(InitialItemStat.Key, InitialItemStat.Value);
	}
}

int32 UBattleInventoryFragment_SetStats::GetItemStatByTag(FGameplayTag Tag) const
{
	if (const int32* StatPtr = InitialItemStats.Find(Tag))
	{
		return *StatPtr;
	}

	return 0;
}
