#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BattleUserFacingExperienceDefinition.generated.h"

UCLASS(BlueprintType)
class LYRABATTLEROYALGAME_API UBattleUserFacingExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Experience, meta=(AllowedTypes="Map"))
	FPrimaryAssetId MapID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Experience, meta=(AllowedTypes="BattleExperienceDefinition"))
	FPrimaryAssetId ExperienceID;
	
};
