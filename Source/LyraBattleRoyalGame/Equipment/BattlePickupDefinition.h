#pragma once

#include "Engine/DataAsset.h"
#include "BattlePickupDefinition.generated.h"

class UBattleInventoryItemDefinition;
class USoundBase;
class UNiagaraSystem;

UCLASS(Blueprintable, BlueprintType, Const, meta=(DisplayName="Battle Pickup Data"))
class LYRABATTLEROYALGAME_API UBattlePickupDefinition : public UDataAsset
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Battle|Pickup|Equipment")
	TSubclassOf<UBattleInventoryItemDefinition> InventoryItemDefinition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Battle|Pickup|Mesh")
	TObjectPtr<UStaticMesh> DisplayMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Battle|Pickup")
	int SpawnCoolDownSeconds;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Battle|Pickup")
	TObjectPtr<USoundBase> PickedUpSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Battle|Pickup")
	TObjectPtr<USoundBase> RespawnedSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Battle|Pickup")
	TObjectPtr<UNiagaraSystem> PickedUpEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle|Pickup")
	TObjectPtr<UNiagaraSystem> RespawnedEffect;
	
};

UCLASS()
class LYRABATTLEROYALGAME_API UBattleWeaponPickupDefinition : public UBattlePickupDefinition
{
	GENERATED_BODY()
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Battle|Pickup|Mesh")
	FVector WeaponMeshOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Battle|Pickup|Mesh")
	FVector WeaponMeshScale = FVector(1.0f,1.0f,1.0f);
};
