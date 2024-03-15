#pragma once

#include "BattleInventoryItemDefinition.generated.h"

/*
 * Inventory에 대한 Fragment에 대한 예시
 * Lyra에서 사용하는 예시
 * - ULyraInventoryFragment_EquippableItem은 EquipmentItemDefinition을 가지고 있고, 장착 가능한 아이템을 의미한다.
 * - ULyraInventoryFragment_SetStats는 아이템에 대한 정보를 가지고 있다.
 * - Rifle에 대한 SetStats으로 총알에 대한 장착 최대치와 남은 잔탄수를 예시로 들수있다.
 */

class UBattleInventoryItemInstance;

UCLASS(Abstract, DefaultToInstanced,EditInlineNew)
class UBattleInventoryItemFragment : public UObject
{
	GENERATED_BODY()
public:
	virtual void OnInstanceCreated(UBattleInventoryItemInstance* Instance) const {}
};


UCLASS(Blueprintable)
class UBattleInventoryItemDefinition : public UObject
{
	GENERATED_BODY()
public:
	UBattleInventoryItemDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	const UBattleInventoryItemFragment* FindFragmentByClass(TSubclassOf<UBattleInventoryItemFragment> FragmentClass) const;
	
	// Inventory Item 정의 이름
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category=Display)
	FText DisplayName;

	// Inventory Item의 COmponent에 대한 Fragment로 인식하면 됨.
	UPROPERTY(EditDefaultsOnly, Instanced, BlueprintReadOnly, Category=Display)
	TArray<TObjectPtr<UBattleInventoryItemFragment>> Fragments;
	
};

