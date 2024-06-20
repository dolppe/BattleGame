#pragma once

#include "BattleContextEffectsLibrary.h"
#include "Engine/DeveloperSettings.h"
#include "GameplayTagContainer.h"
#include "Subsystems/WorldSubsystem.h"
#include "BattleContextEffectsSubsystem.generated.h"

class UAudioComponent;
class UNiagaraComponent;

UCLASS(config = Game, DefaultConfig, meta=(DisplayName="BattleContextEffects"))
class LYRABATTLEROYALGAME_API UBattleContextEffectSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:

	UPROPERTY(config, EditAnywhere)
	TMap<TEnumAsByte<EPhysicalSurface>, FGameplayTag> SurfaceTypeToContextMap;
	
};

UCLASS()
class LYRABATTLEROYALGAME_API UBattleContextEffectSet : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(Transient)
	TSet<TObjectPtr<UBattleContextEffectsLibrary>> BattleContextEffectLibraries;
	
};

UCLASS()
class LYRABATTLEROYALGAME_API UBattleContextEffectsSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, Category = "ContextEffects")
	void SpawnContextEffects(
		const AActor* SpawningActor
		, USceneComponent* AttachToComponent
		, const FName AttachPoint
		, const FVector LocationOffset
		, const FRotator RotationOffset
		, FGameplayTag Effect
		, FGameplayTagContainer Contexts
		, TArray<UAudioComponent*>& AudioOut
		, TArray<UNiagaraComponent*>& NiagaraOut
		, FVector VFXScale = FVector(1)
		, float AudioVolume = 1
		, float AudioPitch = 1);

	UFUNCTION(BlueprintCallable, Category="ContextEffects")
	bool GetContextFromSurfaceType(TEnumAsByte<EPhysicalSurface> PhysicalSurface, FGameplayTag& Context);

	UFUNCTION(BlueprintCallable, Category="ContextEffects")
	void LoadAndAddContextEffectsLibraries(AActor* OwningActor, TSet<TSoftObjectPtr<UBattleContextEffectsLibrary>> ContextEffectsLibraries);

	UFUNCTION(BlueprintCallable, Category="ContextEffects")
	void UnloadAndRemoveContextEffectsLibraries(AActor* OwningActor);

private:

	UPROPERTY(Transient)
	TMap<TObjectPtr<AActor>, TObjectPtr<UBattleContextEffectSet>> ActiveActorEffectsMap;
	
};
