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
