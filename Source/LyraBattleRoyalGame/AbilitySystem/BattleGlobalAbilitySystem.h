#pragma once

#include "Subsystems/WorldSubsystem.h"

#include "Containers/Array.h"
#include "Containers/Map.h"
#include "Containers/SparseArray.h"
#include "GameplayAbilitySpec.h"
#include "Subsystems/WorldSubsystem.h"
#include "Templates/SubclassOf.h"
#include "UObject/UObjectGlobals.h"

#include "BattleGlobalAbilitySystem.generated.h"

struct FGameplayAbilitySpecHandle;
struct FActiveGameplayEffectHandle;
class UBattleAbilitySystemComponent;
class UGameplayEffect;
class UGameplayAbility;

USTRUCT()
struct FGlobalAppliedAbilityList
{
	GENERATED_BODY()

	TMap<TObjectPtr<UBattleAbilitySystemComponent>, FGameplayAbilitySpecHandle> Handles;

	void AddToASC(TSubclassOf<UGameplayAbility> Ability, UBattleAbilitySystemComponent* ASC);
	void RemoveFromASC(UBattleAbilitySystemComponent* ASC);
	void RemoveFromAll();
	
};

USTRUCT()
struct FGlobalAppliedEffectList
{
	GENERATED_BODY()

	UPROPERTY()
	TMap<TObjectPtr<UBattleAbilitySystemComponent>, FActiveGameplayEffectHandle> Handles;

	void AddToASC(TSubclassOf<UGameplayEffect> Effect, UBattleAbilitySystemComponent* ASC);
	void RemoveFromASC(UBattleAbilitySystemComponent* ASC);
	void RemoveFromAll();
};


UCLASS()
class UBattleGlobalAbilitySystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	UBattleGlobalAbilitySystem();
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Battle")
	void ApplyAbilityToAll(TSubclassOf<UGameplayAbility> Ability);
	
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Battle")
	void ApplyEffectToAll(TSubclassOf<UGameplayEffect> Effect);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Battle")
	void RemoveAbilityFromAll(TSubclassOf<UGameplayAbility> Ability);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category="Battle")
	void RemoveEffectFromAll(TSubclassOf<UGameplayEffect> Effect);

	void RegisterASC(UBattleAbilitySystemComponent* ASC);

	void UnregisterASC(UBattleAbilitySystemComponent* ASC);
	
private:

	UPROPERTY()
	TMap<TSubclassOf<UGameplayAbility>, FGlobalAppliedAbilityList> AppliedAbilities;

	UPROPERTY()
	TMap<TSubclassOf<UGameplayEffect>, FGlobalAppliedEffectList> AppliedEffects;

	UPROPERTY()
	TArray<TObjectPtr<UBattleAbilitySystemComponent>> RegisteredASCs;
	
};
