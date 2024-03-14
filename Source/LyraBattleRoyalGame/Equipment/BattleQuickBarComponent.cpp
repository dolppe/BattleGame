#include "BattleQuickBarComponent.h"

#include "BattleEquipmentDefinition.h"
#include "BattleEquipmentInstance.h"
#include "LyraBattleRoyalGame/Equipment/BattleEquipmentManagerComponent.h"
#include "LyraBattleRoyalGame/Inventory/BattleInventoryFragment_EquippableItem.h"
#include "LyraBattleRoyalGame/Inventory/BattleInventoryItemInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleQuickBarComponent)

UBattleQuickBarComponent::UBattleQuickBarComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UBattleQuickBarComponent::BeginPlay()
{
	if (Slots.Num() <NumSlots)
	{
		Slots.AddDefaulted(NumSlots - Slots.Num());
	}
	
	Super::BeginPlay();
}

UBattleEquipmentManagerComponent* UBattleQuickBarComponent::FindEquipmentManager() const
{
	if (AController* OwnerController = Cast<AController>(GetOwner()))
	{
		if (APawn* Pawn = OwnerController->GetPawn())
		{
			return Pawn->FindComponentByClass<UBattleEquipmentManagerComponent>();
		}
	}
	return nullptr;
}

void UBattleQuickBarComponent::EquipItemInSlot()
{
	check(Slots.IsValidIndex(ActiveSlotIndex));
	check(EquippedItem == nullptr);

	// 현재 활성화된 ActiveSlotIndex를 활용하여 활성화된 ItemInstance를 찾음
	if (UBattleInventoryItemInstance* SlotItem = Slots[ActiveSlotIndex])
	{
		// Slot Item을 통해 InventoryFragment_EquippableItem의 Fragment를 찾음.
		// 찾는 것이 실패했으면, 장착할 수 없는 InventoryItem임을 의미함
		if (const UBattleInventoryFragment_EquippableItem* EquipInfo = SlotItem->FindFragmentByClass<UBattleInventoryFragment_EquippableItem>())
		{
			// EquippableItem에서 EquipmentDefinition을 찾는다
			// 이게 있어야 장착 가능
			TSubclassOf<UBattleEquipmentDefinition> EquipDef = EquipInfo->EquipmentDefinition;
			if (EquipDef)
			{
				// 아래는 Unequip이랑 비슷하게 EquipmentManager를 통해 장착한다.
				if (UBattleEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
				{
					EquippedItem = EquipmentManager->EquipItem(EquipDef);
					if (EquippedItem)
					{
						// EquippedItem에는 Instigator로 Slot을 대상으로 넣는다.
						EquippedItem->Instigator = SlotItem;
					}
				}
			}
		}
		
	}
	
}

void UBattleQuickBarComponent::UnequipItemInSlot()
{
	// 참고로 퀵바는 COntroller에 붙어있는 COmponent이고
	// EquipmentManagerCOmponent는 Pawn에 붙어있는 Component임.
	if (UBattleEquipmentManagerComponent* EquipmentManager = FindEquipmentManager())
	{
		// 현재 장착된 Item이 있다면
		if (EquippedItem)
		{
			// EquipmentManager를 통해 해제시킴
			EquipmentManager->UnequipItem(EquippedItem);

			// COntroller에도 EquipItem의 상태를 없는 것으로 업데이트함.
			EquippedItem = nullptr;
		}
	}
	
}

void UBattleQuickBarComponent::AddItemToSlot(int32 SlotIndex, UBattleInventoryItemInstance* Item)
{
	if (Slots.IsValidIndex(SlotIndex) && (Item != nullptr))
	{
		if (Slots[SlotIndex] == nullptr)
		{
			Slots[SlotIndex] = Item;
		}
	}
}

void UBattleQuickBarComponent::SetActiveSlotIndex(int32 NewIndex)
{
	if (Slots.IsValidIndex(NewIndex) && (ActiveSlotIndex != NewIndex))
	{
		// UnequipItem, EquipItem을 통해 NewIndex를 통해 할당된 Item을 장착 및 업데이트 함.
		UnequipItemInSlot();
		ActiveSlotIndex = NewIndex;
		EquipItemInSlot();
		
	}
}
