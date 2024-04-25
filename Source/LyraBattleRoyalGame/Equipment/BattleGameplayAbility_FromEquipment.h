#pragma once

#include "CoreMinimal.h"
#include "LyraBattleRoyalGame/AbilitySystem/Abilities/BattleGameplayAbility.h"
#include "BattleGameplayAbility_FromEquipment.generated.h"

class UBattleInventoryItemInstance;
class UBattleEquipmentInstance;

UCLASS()
class UBattleGameplayAbility_FromEquipment : public UBattleGameplayAbility
{
	GENERATED_BODY()
public:
	UBattleGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="Battle|Ability")
	UBattleEquipmentInstance* GetAssociatedEquipment() const;

	UFUNCTION(BlueprintCallable, Category="Battle|Ability")
	UBattleInventoryItemInstance* GetAssociatedItem() const;
	
};
