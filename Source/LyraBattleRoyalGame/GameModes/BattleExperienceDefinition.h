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

	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TObjectPtr<UBattlePawnData> DefaultPawnData;

	// 활성화할 GameFeature를 설정해둠.
	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TArray<FString> GameFeaturesToEnable;

	UPROPERTY(EditDefaultsOnly, Category=Gameplay)
	TArray<TObjectPtr<UBattleExperienceActionSet>> ActionSets;

	UPROPERTY(EditDefaultsOnly, Category="Actions")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;
};
