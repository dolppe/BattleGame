#pragma once

#include "Components/ControllerComponent.h"
#include "BattleQuickBarComponent.generated.h"

class UBattleEquipmentManagerComponent;
class UBattleInventoryItemInstance;
class UBattleEquipmentInstance;

/*
*	HUD의 QuickBar를 생각하면 됨.
*	- MMORPG에서는 Shortcut HUD
*
*	해당 컴포넌트는 ControllerComponent로서, PlayerController에 의해 조작계 중 하나.
*	- HUD(Slate)로서 Inventory와 Equipment(Gameplay)의 브릿지 역할을 하는 컴포넌트
*/


UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class UBattleQuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()
public:

	UBattleQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	UBattleEquipmentManagerComponent* FindEquipmentManager() const;

	UFUNCTION(BlueprintCallable, Category="Battle")
	void EquipItemInSlot();
	
	UFUNCTION(BlueprintCallable, Category="Battle")
	void UnequipItemInSlot();

	UFUNCTION(BlueprintCallable, Category="Battle")
	void CycleActiveSlotForward();

	UFUNCTION(BlueprintCallable, Category="Battle")
	void CycleActiveSlotBackward();

	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	TArray<UBattleInventoryItemInstance*> GetSlots() const
	{
		return Slots;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	int32 GetNextFreeItemSlot() const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	int32 GetActiveSlotIndex() const {return ActiveSlotIndex; }
	
	UFUNCTION(BlueprintCallable)
	void AddItemToSlot(int32 SlotIndex, UBattleInventoryItemInstance* Item);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	UBattleInventoryItemInstance* RemoveItemFromSlot(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category="Battle")
	void SetActiveSlotIndex(int32 NewIndex);

protected:
	
	// HUD QuickBar Slot 개수
	UPROPERTY()
	int32 NumSlots = 3;

private:
	
	// HUD QuickBar Slot 리스트
	UPROPERTY()
	TArray<TObjectPtr<UBattleInventoryItemInstance>> Slots;
	
	// 현재 활성화된 Slot Index (한번에 하나의 Slot만 활성화 되는 듯?)
	UPROPERTY()
	int32 ActiveSlotIndex = -1;

	// 현재 장착된 장비 정보
	UPROPERTY()
	TObjectPtr<UBattleEquipmentInstance> EquippedItem;

	
};

