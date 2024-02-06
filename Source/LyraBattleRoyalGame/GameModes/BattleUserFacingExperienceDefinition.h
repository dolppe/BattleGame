#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BattleUserFacingExperienceDefinition.generated.h"

class UCommonSession_HostSessionReqeust;

UCLASS(BlueprintType)
class LYRABATTLEROYALGAME_API UBattleUserFacingExperienceDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:

	// Map 로딩 및 Experience 전환을 위해, MapID와 ExperienceID를 활용하여 HosetSessionRequest 생성
	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	UCommonSession_HosetSessionRequest* CreateHostingRequest() const;


	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Experience, meta=(AllowedTypes="Map"))
	FPrimaryAssetId MapID;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category=Experience, meta=(AllowedTypes="BattleExperienceDefinition"))
	FPrimaryAssetId ExperienceID;
	
};
