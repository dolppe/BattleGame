#pragma once

#include "BattleContextEffectsInterface.h"
#include "BattleContextEffectsLibrary.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "BattleContextEffectsComponent.generated.h"


class UAudioComponent;
class UNiagaraComponent;


UCLASS( ClassGroup=(Custom), HideCategories=(Variable, Tags, ComponentTick,ComponentReplication, Activation, Cooking, AssetUserData, Collision), CollapseCategories, meta=(BlueprintSpawnableComponent))
class LYRABATTLEROYALGAME_API UBattleContextEffectComponent : public UActorComponent, public  IBattleContextEffectsInterface
{
	GENERATED_BODY()

public:

	UBattleContextEffectComponent();

protected:

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:

	UFUNCTION(BlueprintCallable)
	virtual void AnimMotionEffect_Implementation(const FName Bone, const FGameplayTag MotionEffect, USceneComponent* StaticMeshComponent, const FVector LocationOffset, const FRotator RotationOffset, const UAnimSequenceBase* AnimationSequence, const bool bHitSuccess, const FHitResult HitResult, FGameplayTagContainer Contexts, FVector VFXScale, float AudioVolume, float AudioPitch) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bConvertPhysicalSurfaceToContext = true;

	UPROPERTY(EditAnywhere)
	FGameplayTagContainer DefaultEffectContexts;

	UPROPERTY(EditAnywhere)
	TSet<TSoftObjectPtr<UBattleContextEffectsLibrary>> DefaultContextEffectsLibraries;

	UFUNCTION(BlueprintCallable)
	void UpdateEffectContexts(FGameplayTagContainer NewEffectContexts);

	UFUNCTION(BlueprintCallable)
	void UpdateLibraries(TSet<TSoftObjectPtr<UBattleContextEffectsLibrary>> NewContextEffectsLibraries);

private:
	UPROPERTY(Transient)
	FGameplayTagContainer CurrentContexts;

	UPROPERTY(Transient)
	TSet<TSoftObjectPtr<UBattleContextEffectsLibrary>> CurrentContextEffectsLibraries;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UAudioComponent>> ActiveAudioComponents;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UNiagaraComponent>> ActiveNiagaraComponents;
	
};
