#pragma once

#include "Containers/Array.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"
#include "GameplayAbilitySpec.h"
#include "Templates/SubclassOf.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "BattleAbilitySet.generated.h"

class UBattleAbilitySystemComponent;
class UBattleGameplayAbility;

USTRUCT(BlueprintType)
struct FBattleAbilitySet_GameplayAbility
{
	GENERATED_BODY()

public:

	// 허용된 어빌리티
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UBattleGameplayAbility> Ability = nullptr;

	// Input 처리를 위한 어빌리티 태그
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag InputTag;

	// 어빌리티 허용 조건 레벨
	UPROPERTY(EditDefaultsOnly)
	int32 AbilityLevel = 1;
	
};

USTRUCT()
struct FBattleAbilitySet_GrantedHandles
{
	GENERATED_BODY()

	void AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle);
	void TakeFromAbilitySystem(UBattleAbilitySystemComponent* BattleASC);
	
protected:
	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> AbilitySpecHandles;
	
};

// Gameplay Ability를 좀 더 쉽게 관리하기 위한 Set
UCLASS(BlueprintType)
class UBattleAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UBattleAbilitySet(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void GiveToAbilitySystem(UBattleAbilitySystemComponent* BattleASC, FBattleAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject= nullptr);
	
	// 허용된 GameplayAbilities
	UPROPERTY(EditDefaultsOnly, Category="Gameplay Abilities")
	TArray<FBattleAbilitySet_GameplayAbility> GrantedGameplayAbilities;
	
};