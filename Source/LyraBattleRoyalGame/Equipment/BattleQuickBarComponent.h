#pragma once

#include "Components/ControllerComponent.h"
#include "BattleQuickBarComponent.generated.h"

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


	// HUD QuickBar Slot 개수
	UPROPERTY()
	int32 NumSlots = 3;

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

