#include "CommonSessionSubsystem.h"
#include "Engine/LocalPlayer.h"
#include "Engine/AssetManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(CommonSessionSubsystem)


FString UCommonSession_HosetSessionRequest::GetMapName() const
{
	// AssetManager를 통해 MapID로 MapAssetData에 정보를 반환하고, 이를 통해 경로를 반환함.
	FAssetData MapAssetData;
	if (UAssetManager::Get().GetPrimaryAssetData(MapID, MapAssetData))
	{
		return MapAssetData.PackageName.ToString();
	}
	return FString();
}

FString UCommonSession_HosetSessionRequest::ConstructTravelURL() const
{
	FString CombinedExtraArgs;

	for (const auto& ExtraArg : ExtraArgs)
	{
		if (ExtraArg.Key.IsEmpty())
		{
			continue;
		}
		/*
		 * ?를 separate로, 복수개의 ExtraArg를 추가함:
		 * Key 값의 유무에 따라
		 * assignment (=)를 통해 알맞는 CmdArg를 생성.
		 */

		if (ExtraArg.Value.IsEmpty())
		{
			CombinedExtraArgs += FString::Printf(TEXT("?%s"), *ExtraArg.Key);
		}
		else
		{
			CombinedExtraArgs += FString::Printf(TEXT("?%s=%s"), *ExtraArg.Key, *ExtraArg.Value);
		}
	}
	
	return FString::Printf(TEXT("%s%s"), *GetMapName(), *CombinedExtraArgs);
}

void UCommonSessionSubsystem::HostSession(APlayerController* HostingPlayer, UCommonSession_HosetSessionRequest* Request)
{
	// 정말 로컬 호스트의 플레이어인지 확인.
	ULocalPlayer* LocalPlayer = (HostingPlayer != nullptr) ? HostingPlayer->GetLocalPlayer() : nullptr;
	if (!LocalPlayer)
	{
		return;
	}

	// Request에서 MapID와 ExtraArgs를 통해 URL을 생성하여 MapLoad 시작
	GetWorld()->ServerTravel(Request->ConstructTravelURL());
}
