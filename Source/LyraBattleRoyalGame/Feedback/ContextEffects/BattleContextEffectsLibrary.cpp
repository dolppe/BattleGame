#include "BattleContextEffectsLibrary.h"

#include "NiagaraSystem.h"
#include "Sound/SoundBase.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleContextEffectsLibrary)

void UBattleContextEffectsLibrary::GetEffects(const FGameplayTag Effect, const FGameplayTagContainer Context,
	TArray<USoundBase*> Sounds, TArray<UNiagaraSystem*>& NiagaraSystems)
{
	if (Effect.IsValid() && Context.IsValid() && EffectsLoadState == EContextEffectsLibraryLoadState::Loaded)
	{
		for (const auto ActiveContextEffect : ActiveContextEffects)
		{
			if (Effect.MatchesTagExact(ActiveContextEffect->EffectTag) 
				&& Context.HasAllExact(ActiveContextEffect->Context)
				&& (ActiveContextEffect->Context.IsEmpty() == Context.IsEmpty()))
			{
				Sounds.Append(ActiveContextEffect->Sounds);
				NiagaraSystems.Append(ActiveContextEffect->NiagaraSystems);
			}
		}
	}
}

void UBattleContextEffectsLibrary::LoadEffects()
{
	if (EffectsLoadState != EContextEffectsLibraryLoadState::Loading)
	{
		EffectsLoadState = EContextEffectsLibraryLoadState::Loading;

		ActiveContextEffects.Empty();

		LoadEffectsInternal();
	}
}

EContextEffectsLibraryLoadState UBattleContextEffectsLibrary::GetContextEffectsLibraryLoadState()
{
	return EffectsLoadState;
}

void UBattleContextEffectsLibrary::LoadEffectsInternal()
{
	TArray<FBattleContextEffects> LocalContextEffects = ContextEffects;

	TArray<UBattleActiveContextEffects*> ActiveContextEffectsArray;

	for (const FBattleContextEffects& ContextEffect : LocalContextEffects)
	{
		if (ContextEffect.EffectTag.IsValid() && ContextEffect.Context.IsValid())
		{
			UBattleActiveContextEffects* NewActiveContextEffects = NewObject<UBattleActiveContextEffects>(this);

			NewActiveContextEffects->EffectTag = ContextEffect.EffectTag;
			NewActiveContextEffects->Context = ContextEffect.Context;

			for (const FSoftObjectPath& Effect : ContextEffect.Effects)
			{
				if (UObject* Object = Effect.TryLoad())
				{
					if (Object->IsA(USoundBase::StaticClass()))
					{
						if (USoundBase* SoundBase = Cast<USoundBase>(Object))
						{
							NewActiveContextEffects->Sounds.Add(SoundBase);
						}
					}
					else if (Object->IsA(UNiagaraSystem::StaticClass()))
					{
						if (UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(Object))
						{
							NewActiveContextEffects->NiagaraSystems.Add(NiagaraSystem);
						}
					}
				}
			}

			ActiveContextEffectsArray.Add(NewActiveContextEffects);
		}
	}

	this->BattleContextEffectLibraryLoadingComplete(ActiveContextEffectsArray);
}

void UBattleContextEffectsLibrary::BattleContextEffectLibraryLoadingComplete(
	TArray<UBattleActiveContextEffects*> BattleActiveContextEffects)
{
	EffectsLoadState = EContextEffectsLibraryLoadState::Loaded;

	ActiveContextEffects.Append(BattleActiveContextEffects);
}
