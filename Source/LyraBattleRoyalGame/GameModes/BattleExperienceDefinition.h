#pragma once

#include "Engine/DataAsset.h"
#include "BattleExperienceDefinition.generated.h"

class UGameFeatureAction;
class UBattlePawnData;
class UBattleExperienceActionSet;

UCLASS(BlueprintType)
class UBattleExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UBattleExperienceDefinition(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

#if WITH_EDITORONLY_DATA
	virtual void UpdateAssetBundleData() override;
#endif
	
	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TObjectPtr<UBattlePawnData> DefaultPawnData;

	// 활성화할 GameFeature를 설정해둠.
	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TArray<FString> GameFeaturesToEnable;

	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TArray<TObjectPtr<UBattleExperienceActionSet>> ActionSets;

	UPROPERTY(EditDefaultsOnly, Instanced, Category="Actions")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};
