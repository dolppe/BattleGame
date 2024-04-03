#include "BattleReticleWidgetBase.h"

#include "LyraBattleRoyalGame/Inventory/BattleInventoryItemInstance.h"
#include "LyraBattleRoyalGame/Weapons/BattleWeaponInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleReticleWidgetBase)

UBattleReticleWidgetBase::UBattleReticleWidgetBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UBattleReticleWidgetBase::InitializeFromWeapon(UBattleWeaponInstance* InWeapon)
{
	WeaponInstance = InWeapon;
	InventoryInstance = nullptr;

	if (WeaponInstance)
	{
		InventoryInstance = Cast<UBattleInventoryItemInstance>(WeaponInstance->GetInstigator());
	}
	OnWeaponInitialized();
}
