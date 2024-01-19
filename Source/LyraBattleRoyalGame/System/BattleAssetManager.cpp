#include "BattleAssetManager.h"
#include "LyraBattleRoyalGame/BattleLogChannels.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleAssetManager)

UBattleAssetManager::UBattleAssetManager() : Super()
{
}

UBattleAssetManager& UBattleAssetManager::Get()
{
	check(GEngine);
	// UEngine의 GEngine에 있는 AssetManager를 상속 받았기에, 이를 그대로 가져오면 됨, 어차피 에셋 매니저는 하나임
	if (UBattleAssetManager* Singleton = Cast<UBattleAssetManager>(GEngine->AssetManager))
	{
		return *Singleton;
	}

	return *NewObject<UBattleAssetManager>();
}

void UBattleAssetManager::AddLoadedAsset(const UObject* Asset)
{
	if (ensureAlways(Asset))
	{
		FScopeLock Lock(&SyncObject);
		LoadedAssets.Add(Asset);
	}
}

bool UBattleAssetManager::ShouldLogAssetLoads()
{
	static bool bLogAssetLoads = FParse::Param(FCommandLine::Get(), TEXT("LogAssetLoads"));
	return bLogAssetLoads;	 
}

UObject* UBattleAssetManager::SynchronousLoadAsset(const FSoftObjectPath& AssetPath)
{
	// 정적 로딩을 불필요하게 하는게 있는지 파악
	if (AssetPath.IsValid())
	{
		// 너무 오래 걸리는 건 동적 로딩으로 하기 위한 로깅
		TUniquePtr<FScopeLogTime> LogTimePtr;
		if (ShouldLogAssetLoads())
		{
			LogTimePtr = MakeUnique<FScopeLogTime>(*FString::Printf(TEXT("synchrounous loaded assets [%s]"), *AssetPath.ToString()), nullptr, FScopeLogTime::ScopeLog_Seconds);
		}

		// AssetManager가 있으면 StreamableManager를 통해 통합적으로 정적 로딩
		// 없으면 FSoftObjectPath를 통해 바로 정적 로딩
		
		if (UAssetManager::IsValid())
		{
			return UAssetManager::GetStreamableManager().LoadSynchronous(AssetPath);
		}
		return AssetPath.TryLoad();
	}
	return nullptr;
}

void UBattleAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	
}
