#pragma once

#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"
#include "BattleWeaponSpawner.generated.h"


class UBattleInventoryItemDefinition;
class UBattleWeaponPickupDefinition;
class UCapsuleComponent;
class APawn;

UCLASS(Blueprintable, BlueprintType)
class LYRABATTLEROYALGAME_API ABattleWeaponSpawner : public AActor
{
	GENERATED_BODY()

public:
	ABattleWeaponSpawner();

	virtual void Tick(float DeltaSeconds) override;
	virtual void OnConstruction(const FTransform& Transform) override;

protected:

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Battle|WeaponPickup")
	TObjectPtr<UBattleWeaponPickupDefinition> WeaponDefinition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Battle|WeaponPickup")
	bool bIsWeaponAvailable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Battle|WeaponPickup")
	float CoolDownTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Battle|WeaponPickup")
	float CheckExistingOverlapDelay;

	UPROPERTY(BlueprintReadOnly, Transient, Category="Battle|WeaponPickup")
	float CoolDownPercentage;
	
public:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Battle|WeaponPickup")
	TObjectPtr<UCapsuleComponent> CollisionVolume;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Battle|WeaponPickup")
	TObjectPtr<UStaticMeshComponent> PadMesh;

	UPROPERTY(BlueprintReadOnly, Category="Battle|WeaponPickup")
	TObjectPtr<UStaticMeshComponent> WeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Battle|WeaponPickup")
	float WeaponMeshRotationSpeed;

	FTimerHandle CoolDownTimerHandle;
	FTimerHandle CheckOverlapsDelayTimerHandle;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	void CheckForExistingOverlaps();
	
	UFUNCTION(BlueprintNativeEvent)
	void AttemptPickUpWeapon(APawn* Pawn);

	UFUNCTION(BlueprintImplementableEvent, Category="Battle|WeaponPickup")
	bool GiveWeapon(TSubclassOf<UBattleInventoryItemDefinition> WeaponItemClass, APawn* ReceivingPawn);

	void SetWeaponPickupVisibility(bool bShouldBeVisible);

	UFUNCTION()
	void OnCoolDownTimerComplete();
	
	void StartCoolDown();

	UFUNCTION(BlueprintCallable, Category = "Battle|WeaponPickup")
	void ResetCoolDown();

	UFUNCTION(BlueprintNativeEvent, Category = "Battle|WeaponPickup")
	void PlayPickupEffects();

	UFUNCTION(BlueprintNativeEvent, Category = "Battle|WeaponPickup")
	void PlayRespawnEffects();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Battle|WeaponPickup")
	static int32 GetDefaultStatFromItemDef(const TSubclassOf<UBattleInventoryItemDefinition> WeaponItemClass, FGameplayTag StatTag);
	
};