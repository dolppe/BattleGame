#pragma once

#include "CoreMinimal.h"
#include "GameFeatureAction_WorldActionBase.h"
#include "Abilities/GameplayAbility.h"
#include "LyraBattleRoyalGame/AbilitySystem/BattleAbilitySet.h"
#include "GameFeatureActon_AddAbilities.generated.h"

class UAttributeSet;
class UDataTable;
struct FComponentRequestHandle;
class UBattleAbilitySet;


USTRUCT(BlueprintType)
struct FBattleAbilityGrant
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AssetBundles="Client,Server"))
	TSoftClassPtr<UGameplayAbility> AbilityType;
	
};

USTRUCT(BlueprintType)
struct FBattleAttributeSetGrant
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AssetBundles="Client,Server"))
	TSoftClassPtr<UAttributeSet> AttributeSetType;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AssetBundles="Client,Server"))
	TSoftObjectPtr<UDataTable> InitializationData;
};


USTRUCT()
struct FGameFeatureAbilitiesEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Abilities")
	TSoftClassPtr<AActor> ActorClass;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<FBattleAbilityGrant> GrantedAbilities;

	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<FBattleAttributeSetGrant> GrantedAttributes;

	UPROPERTY(EditAnywhere, Category="Abilities", meta=(AssetBundles="Client,Server"))
	TArray<TSoftObjectPtr<const UBattleAbilitySet>> GrantedAbilitySets;
	
};



UCLASS()
class UGameFeatureAction_AddAbilities final : public UGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()
public:

	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;

	UPROPERTY(EditAnywhere, Category="Abilities", meta=(TitleProperty="ActorClass", ShowOnlyInnerProperties))
	TArray<FGameFeatureAbilitiesEntry> AbilitiesList;

private:
	struct FActorExtensions
	{
		TArray<FGameplayAbilitySpecHandle> Abilities;
		TArray<UAttributeSet*> Attributes;
		TArray<FBattleAbilitySet_GrantedHandles> AbilitySetHandles;
	};

	struct FPerContextData
	{
		TMap<AActor*, FActorExtensions> ActiveExtensions;
		TArray<TSharedPtr<FComponentRequestHandle>> ComponentRequests;
	};

	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;
	

	void Reset(FPerContextData& ActiveData);
	void HandleActorExtension(AActor* Actor, FName EventName, int32 EntryIndex, FGameFeatureStateChangeContext ChangeContext);
	void AddActorAbilities(AActor* Actor, const FGameFeatureAbilitiesEntry& AbilitiesEntry, FPerContextData& ActiveData);
	void RemoveActorAbilities(AActor* Actor, FPerContextData& ActiveData);

	template<class ComponentType>
	ComponentType* FindOrAddComponentForActor(AActor* Actor, const FGameFeatureAbilitiesEntry& AbilitiesEntry, FPerContextData& ActiveData)
	{
		return Cast<ComponentType>(FindOrAddComponentForActor(ComponentType::StaticClass(), Actor, AbilitiesEntry, ActiveData));
	}
	UActorComponent* FindOrAddComponentForActor(UClass* ComponentType, AActor* Actor, const FGameFeatureAbilitiesEntry& AbilitiesEntry, FPerContextData& ActiveData);

};
