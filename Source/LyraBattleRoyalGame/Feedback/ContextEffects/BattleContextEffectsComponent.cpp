#include "BattleContextEffectsComponent.h"

#include "BattleContextEffectsSubsystem.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleContextEffectsComponent)

UBattleContextEffectComponent::UBattleContextEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = true;
}

void UBattleContextEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentContexts.AppendTags(DefaultEffectContexts);
	CurrentContextEffectsLibraries = DefaultContextEffectsLibraries;

	if (const UWorld* World = GetWorld())
	{
		if (UBattleContextEffectsSubsystem* BattleContextEffectsSubsystem = World->GetSubsystem<UBattleContextEffectsSubsystem>())
		{
			BattleContextEffectsSubsystem->LoadAndAddContextEffectsLibraries(GetOwner(), CurrentContextEffectsLibraries);
		}
	}
}

void UBattleContextEffectComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (const UWorld* World = GetWorld())
	{
		if (UBattleContextEffectsSubsystem* BattleContextEffectsSubsystem = World->GetSubsystem<UBattleContextEffectsSubsystem>())
		{
			BattleContextEffectsSubsystem->UnloadAndRemoveContextEffectsLibraries(GetOwner());
		}
	}
	
	Super::EndPlay(EndPlayReason);
}

void UBattleContextEffectComponent::AnimMotionEffect_Implementation(const FName Bone, const FGameplayTag MotionEffect,
	USceneComponent* StaticMeshComponent, const FVector LocationOffset, const FRotator RotationOffset,
	const UAnimSequenceBase* AnimationSequence, const bool bHitSuccess, const FHitResult HitResult,
	FGameplayTagContainer Contexts, FVector VFXScale, float AudioVolume, float AudioPitch)
{
	TArray<UAudioComponent*> AudioComponentsToAdd;
	TArray<UNiagaraComponent*> NiagaraComponentsToAdd;

	FGameplayTagContainer TotalContexts;

	TotalContexts.AppendTags(Contexts);
	TotalContexts.AppendTags(CurrentContexts);

	if (bConvertPhysicalSurfaceToContext)
	{
		TWeakObjectPtr<UPhysicalMaterial> PhysicalSurfaceTypePtr = HitResult.PhysMaterial;

		if (PhysicalSurfaceTypePtr.IsValid())
		{
			TEnumAsByte<EPhysicalSurface> PhysicalSurfaceType = PhysicalSurfaceTypePtr->SurfaceType;
			
			if (const UBattleContextEffectSettings* BattleContextEffectSettings = GetDefault<UBattleContextEffectSettings>())
			{
				if (const FGameplayTag* SurfaceContextPtr = BattleContextEffectSettings->SurfaceTypeToContextMap.Find(PhysicalSurfaceType))
				{
					FGameplayTag SurfaceContext = *SurfaceContextPtr;

					TotalContexts.AddTag(SurfaceContext);
				}
			}
		}
	}

	for (UAudioComponent* ActiveAudioComponent : ActiveAudioComponents)
	{
		if (ActiveAudioComponent)
		{
			AudioComponentsToAdd.Add(ActiveAudioComponent);
		}
	}

	for (UNiagaraComponent* ActiveNiagaraComponent : ActiveNiagaraComponents)
	{
		if (ActiveNiagaraComponent)
		{
			NiagaraComponentsToAdd.Add(ActiveNiagaraComponent);
		}
	}

	if (const UWorld* World = GetWorld())
	{
		if (UBattleContextEffectsSubsystem* BattleContextEffectsSubsystem = World->GetSubsystem<UBattleContextEffectsSubsystem>())
		{
			TArray<UAudioComponent*> AudioComponents;
			TArray<UNiagaraComponent*> NiagaraComponents;

			BattleContextEffectsSubsystem->SpawnContextEffects(GetOwner(), StaticMeshComponent, Bone, 
				LocationOffset, RotationOffset, MotionEffect, TotalContexts,
				AudioComponents, NiagaraComponents, VFXScale, AudioVolume, AudioPitch);

			AudioComponentsToAdd.Append(AudioComponents);
			NiagaraComponentsToAdd.Append(NiagaraComponents);
		}
	}

	ActiveAudioComponents.Empty();
	ActiveAudioComponents.Append(AudioComponentsToAdd);

	ActiveNiagaraComponents.Empty();
	ActiveNiagaraComponents.Append(NiagaraComponentsToAdd);
	
	
}

void UBattleContextEffectComponent::UpdateEffectContexts(FGameplayTagContainer NewEffectContexts)
{
	CurrentContexts.Reset(NewEffectContexts.Num());
	CurrentContexts.AppendTags(NewEffectContexts);
}

void UBattleContextEffectComponent::UpdateLibraries(
	TSet<TSoftObjectPtr<UBattleContextEffectsLibrary>> NewContextEffectsLibraries)
{
	CurrentContextEffectsLibraries = NewContextEffectsLibraries;
	
	if (const UWorld* World = GetWorld())
	{
		if (UBattleContextEffectsSubsystem* LyraContextEffectsSubsystem = World->GetSubsystem<UBattleContextEffectsSubsystem>())
		{
			// Load and Add Libraries to Subsystem                  
			LyraContextEffectsSubsystem->LoadAndAddContextEffectsLibraries(GetOwner(), CurrentContextEffectsLibraries);
		}
	}
}
