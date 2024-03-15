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

	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category=Animation)
	TSubclassOf<UAnimInstance> PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags)const;
	
	// Weapon에 Equip, UnEquip에 대한 Animation Set 정보를 들고 있음.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation)
	FBattleAnimLayerSelectionSet EquippedAnimSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation)
	FBattleAnimLayerSelectionSet UnequippedAnimSet;
	
};
