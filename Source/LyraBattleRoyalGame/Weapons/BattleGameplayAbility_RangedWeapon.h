#pragma once

#include "CoreMinimal.h"
#include "LyraBattleRoyalGame/Equipment/BattleGameplayAbility_FromEquipment.h"
#include "BattleGameplayAbility_RangedWeapon.generated.h"

UCLASS()
class UBattleGameplayAbility_RangedWeapon : public UBattleGameplayAbility_FromEquipment
{
	GENERATED_BODY()
public:
	UBattleGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};
