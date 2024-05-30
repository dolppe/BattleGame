#pragma once

#include "GameplayTagContainer.h"
#include "BattleInventoryItemDefinition.h"
#include "BattleInventoryFragment_SetStats.generated.h"

class UBattleInventoryItemInstance;

UCLASS()
class UBattleInventoryFragment_SetStats : public UBattleInventoryItemFragment
{
	GENERATED_BODY()
public:

	virtual void OnInstanceCreated(UBattleInventoryItemInstance* Instance) const override;

	int32 GetItemStatByTag(FGameplayTag Tag) const;
	
	UPROPERTY(EditDefaultsOnly, Category=Equipment)
	TMap<FGameplayTag, int32> InitialItemStats;
	
};


