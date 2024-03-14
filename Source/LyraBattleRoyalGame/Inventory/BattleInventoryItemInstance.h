#pragma once

#include "UObject/Object.h"
#include "Templates/SubclassOf.h"
#include "BattleInventoryItemInstance.generated.h"

class UBattleInventoryItemDefinition;
class UBattleInventoryItemFragment;

// 해당 클래스는 Inventory Item의 Instance임.
UCLASS(BlueprintType)
class UBattleInventoryItemInstance : public UObject
{
	GENERATED_BODY()
public:
	UBattleInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintPure = false, meta=(DeterminesOutputType=FragmentClass))
	const UBattleInventoryItemFragment* FindFragmentByClass(TSubclassOf<UBattleInventoryItemFragment> FragmentClass) const;
	
	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*) FindFragmentByClass(ResultClass::StaticClass());
	}
	
	// 인벤토리의 아이템 인스턴스는 무엇으로 정의되어있는지 메타 클래스인 Definition을 들고 있음.
	UPROPERTY()
	TSubclassOf<UBattleInventoryItemDefinition> ItemDef;
	
};

