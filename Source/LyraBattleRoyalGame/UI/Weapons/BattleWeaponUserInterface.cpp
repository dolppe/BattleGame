#include "BattleWeaponUserInterface.h"

#include "LyraBattleRoyalGame/Equipment/BattleEquipmentManagerComponent.h"
#include "LyraBattleRoyalGame/Weapons/BattleWeaponInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleWeaponUserInterface)

UBattleWeaponUserInterface::UBattleWeaponUserInterface(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UBattleWeaponUserInterface::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (APawn* Pawn = GetOwningPlayerPawn())
	{
		if (UBattleEquipmentManagerComponent* EquipmentManager = Pawn->FindComponentByClass<UBattleEquipmentManagerComponent>())
		{
			if (UBattleWeaponInstance* NewInstance = EquipmentManager->GetFirstInstanceOfType<UBattleWeaponInstance>())
			{
				if (NewInstance != CurrentInstance && NewInstance->GetInstigator() != nullptr)
				{
					UBattleWeaponInstance* OldWeapon = CurrentInstance;
					CurrentInstance = NewInstance;
					OnWeaponChanged(OldWeapon, CurrentInstance);
					
				}
			}
		}
	}
}
