#pragma once

#include "LyraBattleRoyalGame/Equipment/BattleEquipmentInstance.h"
#include "LyraBattleRoyalGame/Cosmetics/BattleCosmeticAnimationTypes.h"
#include "BattleWeaponInstance.generated.h"


UCLASS()
class UBattleWeaponInstance : public UBattleEquipmentInstance
{
	GENERATED_BODY()
public:
	UBattleWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation)
	FBattleAnimLayerSelectionSet EquippedAnimSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation)
	FBattleAnimLayerSelectionSet UnequippedAnimSet;
};


ww