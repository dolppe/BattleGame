#include "BattleExperienceActionSet.h"

#include "GameFeatureAction.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleExperienceActionSet)

UBattleExperienceActionSet::UBattleExperienceActionSet()
	: Super()
{
}

#if WITH_EDITORONLY_DATA
void UBattleExperienceActionSet::UpdateAssetBundleData()
{
	Super::UpdateAssetBundleData();

	for (UGameFeatureAction* Action : Actions)
	{
		if (Action)
		{
			Action->AddAdditionalAssetBundleData(AssetBundleData);
		}
	}
}
#endif

