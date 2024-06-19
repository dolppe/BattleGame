#pragma once

#include "GameplayTagContainer.h"
#include "BattleContextEffectsLibrary.generated.h"

class UNiagaraSystem;

UENUM()
enum class EContextEffectsLibraryLoadState : uint8
{
	Unloaded = 0,
	Loading = 1,
	Loaded = 2
};

USTRUCT(BlueprintType)
struct LYRABATTLEROYALGAME_API FBattleContextEffects
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag EffectTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTagContainer Context;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowedClasses = "/Script/Engine.SoundBase, /Script/Niagara.NiagaraSystem"))
	TArray<FSoftObjectPath> Effects;
	
};



UCLASS()
class LYRABATTLEROYALGAME_API UBattleActiveContextEffects : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(VisibleAnywhere)
	FGameplayTag EffectTag;

	UPROPERTY(VisibleAnywhere)
	FGameplayTagContainer Context;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<USoundBase>> Sounds;

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UNiagaraSystem>> NiagaraSystems;
};

DECLARE_DYNAMIC_DELEGATE_OneParam(FBattleContexteEffectLibraryLoadingComplete, TArray<UBattleActiveContextEffects*>, BattleActiveContexteEffects);

UCLASS(BlueprintType)
class LYRABATTLEROYALGAME_API UBattleContextEffectsLibrary : public UObject
{
	GENERATED_BODY()

public:

	// 여기에 넣어두면 로딩해서 Active에 담음.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FBattleContextEffects> ContextEffects;

	UFUNCTION(BlueprintCallable)
	void GetEffects(const FGameplayTag Effect, const FGameplayTagContainer Context, TArray<USoundBase*> Sounds, TArray<UNiagaraSystem*>& NiagaraSystems);

	UFUNCTION(BlueprintCallable)
	void LoadEffects();

	EContextEffectsLibraryLoadState GetContextEffectsLibraryLoadState();

private:
	void LoadEffectsInternal();

	void BattleContextEffectLibraryLoadingComplete(TArray<UBattleActiveContextEffects*> BattleActiveContextEffects);

	UPROPERTY(Transient)
	TArray<TObjectPtr<UBattleActiveContextEffects>> ActiveContextEffects;

	UPROPERTY(Transient)
	EContextEffectsLibraryLoadState EffectsLoadState = EContextEffectsLibraryLoadState::Unloaded;
	
};
