#pragma once

#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "BattleAbilityTagRelationshipMapping.generated.h"

USTRUCT()
struct FBattleAbilityTagRelationship
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Ability, meta = (Categories = "Gameplay.Action"))
	FGameplayTag AbilityTag;

	// 메인 Tag가 활성화 될때 설정된 태그들은 Block됨
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer AbilityTagsToBlock;

	// 메인 Tag가 활성화 될때 설정된 태그들은 Cancel됨.
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer AbilityTagsToCancel;

	// 메인 태그가 활성화되기 위해 필요한 태그
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer ActivationRequiredTags;

	// 해당 태그가 있으면 메인 태그 활성화가 안됨.
	UPROPERTY(EditAnywhere, Category = Ability)
	FGameplayTagContainer ActivationBlockedTags;	
};


UCLASS()
class UBattleAbilityTagRelationshipMapping : public UDataAsset
{
	GENERATED_BODY()

private:
	
	UPROPERTY(EditAnywhere, Category=Ability, meta=(TitleProperty="AbilityTag"))
	TArray<FBattleAbilityTagRelationship> AbilityTagRelationships;

public:
	
	void GetAbilityTagsToBlockAndCancel(const FGameplayTagContainer& AbilityTags, FGameplayTagContainer* OutTagsToBlock, FGameplayTagContainer* OutTagsToCancel) const;
	
	void GetRequiredAndBlockedActivationTags(const FGameplayTagContainer& AbilityTags, FGameplayTagContainer* OutActivationRequired, FGameplayTagContainer* OutActivationBlocked) const;
	
};
