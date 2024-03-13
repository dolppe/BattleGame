#pragma once

#include "GameplayTagContainer.h"
#include "BattleCosmeticAnimationTypes.generated.h"

USTRUCT(BlueprintType)
struct FBattleAnimLayerSelectionEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> Layer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer RequiredTags;
	
};

USTRUCT(BlueprintType)
struct FBattleAnimLayerSelectionSet
{
	GENERATED_BODY()

	TSubclassOf<UAnimInstance> SelectBestLayer(const FGameplayTagContainer& CosmeticTags) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FBattleAnimLayerSelectionEntry> LayerRules;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> DefaultLayer;
	
};


USTRUCT(BlueprintType)
struct FBattleAnimBodyStyleSelectionEntry
{
	GENERATED_BODY()

	// Target SkeletalMesh to apply AnimLayer
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> Mesh = nullptr;
	
	// Cosmetic Tag
	// To Part restriction
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Categories = "Cosmetic"))
	FGameplayTagContainer RequiredTags;
	
};

USTRUCT(BlueprintType)
struct FBattleAnimBodyStyleSelectionSet
{
	GENERATED_BODY()

	// GameplayTag를 통해 (CosmeticTags), Mesh Rules에 따라 알맞은 BodyStyle로 변환함
	USkeletalMesh* SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const;

	// AnimLayer 적용할 SkeletalMesh를 들고 있음.
	// Animation-Mesh간 Rules를 MeshRules라고 생각하면 됨.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FBattleAnimBodyStyleSelectionEntry> MeshRules;

	// Default SkeletalMesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMesh> DefaultMesh = nullptr;

	// Physics Asset은 하나로 동일함
	// 즉, 모든 Animation의 Physics 속성은 공유함.
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPhysicsAsset> ForcedPhysicsAsset = nullptr;
	
};