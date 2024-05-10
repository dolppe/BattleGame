#include "BattleWeaponStateComponent.h"

#include "BattleRangedWeaponInstance.h"
#include "LyraBattleRoyalGame/Equipment/BattleEquipmentManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleWeaponStateComponent)

UBattleWeaponStateComponent::UBattleWeaponStateComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UBattleWeaponStateComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (APawn* Pawn = GetPawn<APawn>())
	{
		if (UBattleEquipmentManagerComponent* EquipmentManager = Pawn->FindComponentByClass<UBattleEquipmentManagerComponent>())
		{
			if (UBattleRangedWeaponInstance* CurrentWeapon = Cast<UBattleRangedWeaponInstance>(EquipmentManager->GetFirstInstanceOfType(UBattleRangedWeaponInstance::StaticClass())))
			{
				CurrentWeapon->Tick(DeltaTime);
			}
		}
	}
}

