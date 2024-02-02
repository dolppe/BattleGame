#include "BattleExperienceManagerComponent.h"

#include "GameFeaturesSubsystemSettings.h"
#include "LyraBattleRoyalGame/GameModes/BattleExperienceDefinition.h"
#include "LyraBattleRoyalGame/System/BattleAssetManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleExperienceManagerComponent)

UBattleExperienceManagerComponent::UBattleExperienceManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UBattleExperienceManagerComponent::CallOrRegister_OnExperienceLoaded(
	FOnBattleExperienceLoaded::FDelegate&& Delegate)
{
	if (IsExperienceLoaded())
	{
		Delegate.Execute(CurrentExperience);
	}
	else
	{
		OnExperienceLoaded.Add(MoveTemp(Delegate));
	}
	
}


void UBattleExperienceManagerComponent::ServerSetCurrentExperience(FPrimaryAssetId ExperienceId)
{
	// GameMode에서 넣은 Default Experience를 활용하여 만든 PrimaryAssetID를 활용하여 CDO를 가져옴.
	UBattleAssetManager& AssetManager = UBattleAssetManager::Get();

	TSubclassOf<UBattleExperienceDefinition> AssetClass;
	{
		FSoftObjectPath AssetPath = AssetManager.GetPrimaryAssetPath(ExperienceId);
		AssetClass = Cast<UClass>(AssetPath.TryLoad());
	}

	// CDO를 가져옴.
	// CDO를 사용하는 이유는 추후 GetPrimaryAssetId 함수를 통해 PrimaryAssetId를 가져와야 하는데,
	// 이때 CDO를 사용해야 제대로 동작함. (내부 함수임)
	const UBattleExperienceDefinition* Experience = GetDefault<UBattleExperienceDefinition>(AssetClass);
	check(Experience != nullptr);
	check(CurrentExperience == nullptr);
	{
		CurrentExperience = Experience;
	}

	StartExperienceLoad();	
}

void UBattleExperienceManagerComponent::StartExperienceLoad()
{
	check(CurrentExperience);
	check(LoadState == EBattleExperienceLoadState::Unloaded);

	LoadState = EBattleExperienceLoadState::Loading;

	UBattleAssetManager& AssetManager = UBattleAssetManager::Get();

	TSet<FPrimaryAssetId> BundleAssetList;

	// 현재 전달한 CurrentExperience는 처음 Experience BP를 가져와서 PrimaryAssetId로 만들고,
	// 이를 이용하여 CDO를 가져온 것임. => GetPrimaryAssetId 내부 구조 떄문임.
	// GetPrimaryAssetId
	// - 내부를 확인하면 우선 CDO인지를 먼저 확인하고 확인이 되어야 다음을 제대로 탐색함.
	// - 해당 CDO의 Parent Class를 확인해서 가장 먼저 Native C++ Class를 만난 경우 이에 대한 AssetId를 반환
	// - 없는 경우에는 가장 최상위의 BP Class를 이용해서 AssetId를 가져옴.
	// - 또한 최종적으로 선택된 클래스의 부모 클래스가 UPrimaryDataAsset인지 확인함.
	// 즉, CDO로 설정해야 제대로 가져와짐.
	// 내부적으로 부모 클래스 중 UPrimaryDataAsset을 상속 받은 클래스가 있는지 확인하기 위해 이러한 과정을 거치면서 체크하는 것.
	// => UPrimaryDataAsset을 상속 받은 클래스 자체로는 의미가 없기 때문
	// ex) BattleExperienceDefinition => 이것 자체로는 Data가 들어있지 않음 (정의만 있음)
	// 이를 상속 받아서 데이터를 채워둔 클래스여야 AssetID를 찾을 수 있음 ( ex) BP_DefaultExperience)
	// 결국, UPrimaryDataAsset을 상속 받은 클래스를 상속 받은 구체적인 클래스를 찾는 것이 목적임
	// ex) UPrimaryDataAsset -> ExpereinceDefinition -> BP_DefaultExpereince Class
	BundleAssetList.Add(CurrentExperience->GetPrimaryAssetId());

	
	TArray<FName> BundlesToLoad;
	{
		const ENetMode OwnerNetMode = GetOwner()->GetNetMode();
		bool bLoadClient = GIsEditor || (OwnerNetMode != NM_DedicatedServer);
		bool bLoadServer = GIsEditor || (OwnerNetMode != NM_Client);
		if (bLoadClient)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateClient);
		}
		if (bLoadServer)
		{
			BundlesToLoad.Add(UGameFeaturesSubsystemSettings::LoadStateServer);
		}
	}

	FStreamableDelegate OnAssetsLoadedDelegate = FStreamableDelegate::CreateUObject(this, &ThisClass::OnExperienceLoadComplete);
	
	TSharedPtr<FStreamableHandle> Handle = AssetManager.ChangeBundleStateForPrimaryAssets(
		BundleAssetList.Array(),
		BundlesToLoad,
		{}, false, FStreamableDelegate(), FStreamableManager::AsyncLoadHighPriority);

	if (!Handle.IsValid() || Handle->HasLoadCompleted())
	{
		// Delegate를 바로 호출 하거나 다음 프레임 혹은 현재 프레임의 마지막에 호출될 예정임.
		// => 여기서는 설정상 다음 프레임이나 현재 프레임의 마지막에 호출될 것.
		//  - 내부적으로 StreamableDelegateDelayHelper를 활용하여 미루는 것임
		FStreamableHandle::ExecuteDelegate(OnAssetsLoadedDelegate);
	}
	else
	{
		Handle->BindCompleteDelegate(OnAssetsLoadedDelegate);
		Handle->BindCancelDelegate(FStreamableDelegate::CreateLambda([OnAssetsLoadedDelegate]()
			{
				OnAssetsLoadedDelegate.ExecuteIfBound();
			}));
	}
	
}

void UBattleExperienceManagerComponent::OnExperienceLoadComplete()
{
	check(LoadState == EBattleExperienceLoadState::Loading);
	check(CurrentExperience);

	// 여기에는 Experience에 등록된 GameFeature Plugin에 대한 처리를 진행함.
	// 이는 추후 분석후 추가할 예정
	
	OnExperienceFullLoadCompleted();
}

void UBattleExperienceManagerComponent::OnExperienceFullLoadCompleted()
{
	check(LoadState != EBattleExperienceLoadState::Loaded);

	// Experience가 완전히 로드 됐으니 등록된 델리게이트들에게 Broadcast 진행.
	LoadState = EBattleExperienceLoadState::Loaded;
	OnExperienceLoaded.Broadcast(CurrentExperience);
	OnExperienceLoaded.Clear();
}

const UBattleExperienceDefinition* UBattleExperienceManagerComponent::GetCurrentExperienceChecked() const
{
	check(LoadState == EBattleExperienceLoadState::Loaded);
	check(CurrentExperience != nullptr);

	return CurrentExperience;
}