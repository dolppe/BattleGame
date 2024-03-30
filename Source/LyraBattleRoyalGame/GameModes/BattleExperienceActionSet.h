#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BattleExperienceActionSet.generated.h"


class UGameFeatureAction;

UCLASS()
class UBattleExperienceActionSet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UBattleExperienceActionSet();

#if WITH_EDITORONLY_DATA
	virtual void UpdateAssetBundleData() override;
#endif

	UPROPERTY(EditAnywhere, Category="Actions to Perform")
	TArray<TObjectPtr<UGameFeatureAction>> Actions;	
};

