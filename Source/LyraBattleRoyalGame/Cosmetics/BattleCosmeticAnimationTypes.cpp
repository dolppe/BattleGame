#include "BattleCosmeticAnimationTypes.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleCosmeticAnimationTypes)

TSubclassOf<UAnimInstance> FBattleAnimLayerSelectionSet::SelectBestLayer(
	const FGameplayTagContainer& CosmeticTags) const
{
	for (const FBattleAnimLayerSelectionEntry& Rule : LayerRules)
	{
		if ((Rule.Layer != nullptr) && CosmeticTags.HasAll(Rule.RequiredTags))
		{
			return Rule.Layer;
		}
	}
	return DefaultLayer;
}

USkeletalMesh* FBattleAnimBodyStyleSelectionSet::SelectBestBodyStyle(const FGameplayTagContainer& CosmeticTags) const
{
	// MeshRule을 순회하며 CosmeticTags 요구 조건에 맞는 MeshRule을 찾아 SkeletalMesh를 반환함.
	for (const FBattleAnimBodyStyleSelectionEntry& Rule : MeshRules)
	{
		if ((Rule.Mesh)&& CosmeticTags.HasAll(Rule.RequiredTags))
		{
			return Rule.Mesh;
		}
	}
	return DefaultMesh;
}
