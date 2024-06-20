#include "BattleContextEffectsSubsystem.h"

#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleContextEffectsSubsystem)

void UBattleContextEffectsSubsystem::SpawnContextEffects(const AActor* SpawningActor,
	USceneComponent* AttachToComponent, const FName AttachPoint, const FVector LocationOffset,
	const FRotator RotationOffset, FGameplayTag Effect, FGameplayTagContainer Contexts,
	TArray<UAudioComponent*>& AudioOut, TArray<UNiagaraComponent*>& NiagaraOut, FVector VFXScale, float AudioVolume,
	float AudioPitch)
{
	if (TObjectPtr<UBattleContextEffectSet>* EffectsLibrariesSetPtr = ActiveActorEffectsMap.Find(SpawningActor))
	{
		if (UBattleContextEffectSet* EffectLibraries = *EffectsLibrariesSetPtr)
		{
			TArray<USoundBase*> TotalSounds;
			TArray<UNiagaraSystem*> TotalNiagaraSystem;

			for (UBattleContextEffectsLibrary* EffectLibrary : EffectLibraries->BattleContextEffectLibraries)
			{
				if (EffectLibrary && EffectLibrary->GetContextEffectsLibraryLoadState() == EContextEffectsLibraryLoadState::Loaded)
				{
					TArray<USoundBase*> Sounds;
					TArray<UNiagaraSystem*> NiagaraSystems;

					EffectLibrary->GetEffects(Effect, Contexts, Sounds, NiagaraSystems);

					TotalSounds.Append(Sounds);
					TotalNiagaraSystem.Append(NiagaraSystems);
				}
				else if (EffectLibrary && EffectLibrary->GetContextEffectsLibraryLoadState() == EContextEffectsLibraryLoadState::Unloaded)
				{
					EffectLibrary->LoadEffects();
				}
			}

			for (USoundBase* Sound : TotalSounds)
			{
				UAudioComponent* AudioComponent = UGameplayStatics::SpawnSoundAttached(Sound, AttachToComponent, AttachPoint, LocationOffset, RotationOffset, EAttachLocation::KeepRelativeOffset,
						false, AudioVolume, AudioPitch, 0.0f, nullptr, nullptr, true);

				AudioOut.Add(AudioComponent);
			}

			for (UNiagaraSystem* NiagaraSystem : TotalNiagaraSystem)
			{
				// Spawn Niagara Systems Attached, add Niagara Component to List of NCs
				UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(NiagaraSystem, AttachToComponent, AttachPoint, LocationOffset,
					RotationOffset, VFXScale, EAttachLocation::KeepRelativeOffset, true, ENCPoolMethod::None, true, true);

				NiagaraOut.Add(NiagaraComponent);
			}
		}
	}
}

bool UBattleContextEffectsSubsystem::GetContextFromSurfaceType(TEnumAsByte<EPhysicalSurface> PhysicalSurface,
	FGameplayTag& Context)
{
	if (const UBattleContextEffectSettings* ProjectSettings = GetDefault<UBattleContextEffectSettings>())
	{
		if (const FGameplayTag* GameplayTagPtr = ProjectSettings->SurfaceTypeToContextMap.Find(PhysicalSurface))
		{
			Context = *GameplayTagPtr;
		}
	}

	return Context.IsValid();
}

void UBattleContextEffectsSubsystem::LoadAndAddContextEffectsLibraries(AActor* OwningActor,
	TSet<TSoftObjectPtr<UBattleContextEffectsLibrary>> ContextEffectsLibraries)
{
	if (OwningActor == nullptr || ContextEffectsLibraries.Num() <= 0)
	{
		return;
	}

	UBattleContextEffectSet* EffectsLibrariesSet = NewObject<UBattleContextEffectSet>(this);

	for (const TSoftObjectPtr<UBattleContextEffectsLibrary>& ContextEffectSoftObj : ContextEffectsLibraries)
	{
		if (UBattleContextEffectsLibrary* EffectsLibrary = ContextEffectSoftObj.LoadSynchronous())
		{
			EffectsLibrary->LoadEffects();

			EffectsLibrariesSet->BattleContextEffectLibraries.Add(EffectsLibrary);
		}
	}

	ActiveActorEffectsMap.Emplace(OwningActor, EffectsLibrariesSet );
		
}

void UBattleContextEffectsSubsystem::UnloadAndRemoveContextEffectsLibraries(AActor* OwningActor)
{
	if (OwningActor == nullptr)
	{
		return;
	}

	ActiveActorEffectsMap.Remove(OwningActor);
}
