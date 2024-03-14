#pragma once

#include "UObject/Object.h"
#include "BattleEquipmentInstance.generated.h"

struct FBattleEquipmentActorToSpawn;

UCLASS(BlueprintType, Blueprintable)
class UBattleEquipmentInstance : public UObject
{
	GENERATED_BODY()
public:
	UBattleEquipmentInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintPure, Category=Equipment)
	APawn* GetPawn() const;

	// DeterminesOutputType은 C++ 정의에는 APawn* 반환하지만, BP에서는 PawnType에 따라 OutputType이 결정되도록 Redirect함
	UFUNCTION(BlueprintPure, Category=Equipment, meta=(DeterminesOutputType = PawnType))
	APawn* GetTypedPawn(TSubclassOf<APawn> PawnType) const;

	UFUNCTION(BlueprintPure, Category=Equipment)
	UObject* GetInstigator() const { return Instigator;}
	
	void SpawnEquipmentActors(const TArray<FBattleEquipmentActorToSpawn>& ActorsToSpawn);	
	void DestroyEquipmentActors();
	
	UFUNCTION(BlueprintImplementableEvent, Category=Equipment, meta = (DisplayName = "OnEquipped"))
	void K2_OnEquipped();

	UFUNCTION(BlueprintImplementableEvent, Category=Equipment, meta = (DisplayName = "OnUnequipped"))
	void K2_OnUnequipped();

	UFUNCTION(BlueprintPure, Category=Equipment)
	TArray<AActor*> GetSpawnedActors() const {return SpawnedActors;}

	virtual void OnEquipped();
	virtual void OnUnequipped();
	
	UPROPERTY()
	TObjectPtr<UObject> Instigator;

	// Definition에 맞게 Spawn된 Actor들.
	UPROPERTY()
	TArray<TObjectPtr<AActor>> SpawnedActors;
	
};

