#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CommonSessionSubsystem.generated.h"

class APlayerController;

// UCommonSession_HosetSessionRequest를 CommonSessionSubsystem에 전달하면,
// CommonSessionSubsystem에서 MapID와 CmrArgs로 최종적인 CmdArgs를 만들어서 맵 로딩.
// 맵 전환하기 위한 요청일 것임. (처음에는 로드)

UCLASS(BlueprintType)
class COMMONUSER_API UCommonSession_HosetSessionRequest : public UObject
{
	GENERATED_BODY()
public:
	// MapID -> Text로 변환
	FString GetMapName() const;
	
	// 최종 전달할 URL 생성.
	FString ConstructTravelURL() const;
	
	// 준비할 MapID (맵 경로)
	UPROPERTY(BlueprintReadWrite, Category= Session)
	FPrimaryAssetId MapID;

	// 전달할 CmdArgs (Experience 이름을 전달)
	// => UserFacingExperienceDefinition에 추가할 것임.
	UPROPERTY(BlueprintReadWrite, Category= Session)
	TMap<FString, FString> ExtraArgs;
	
};

UCLASS()
class COMMONUSER_API UCommonSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UCommonSessionSubsystem() {}

	UFUNCTION(BlueprintCallable, Category=Session)
	void HostSession(APlayerController* HostingPlayer, UCommonSession_HosetSessionRequest* Request);
	
	// 맵 경로
	FString PendingTravelURL;
};
