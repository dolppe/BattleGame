#pragma once

#include "Containers/Array.h"
#include "LyraBattleRoyalGame/Inventory/BattleInventoryItemDefinition.h"
#include "Templates/SubclassOf.h"
#include "InventoryFragment_ReticleConfig.generated.h"

class UBattleReticleWidgetBase;

UCLASS()
class UBattleInventoryFragment_ReticleConfig : public UBattleInventoryItemFragment
{
	GENERATED_BODY()
public:
	
	// 무기에 결합된 ReticleWidget 정보를 가지고 있는 프레그먼트
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Reticle)
	TArray<TSubclassOf<UBattleReticleWidgetBase>> ReticleWidgets;
};

