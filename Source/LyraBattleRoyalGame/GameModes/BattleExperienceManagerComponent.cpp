#include "BattleExperienceManagerComponent.h"

#include "GameFeaturesSubsystem.h"
#include "GameFeaturesSubsystemSettings.h"
#include "LyraBattleRoyalGame/GameModes/BattleExperienceDefinition.h"
#include "BattleExperienceActionSet.h"
#include "LyraBattleRoyalGame/BattleLogChannels.h"
#include "LyraBattleRoyalGame/System/BattleAssetManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleExperienceManagerComponent)

UBattleExperienceManagerComponent::UBattleExperienceManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UBattleExperienceManagerComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	for (const FString& PluginURL : GameFeaturePluginURLs)
	{
		UGameFeaturesSubsystem::Get().DeactivateGameFeaturePlugin(PluginURL);
	}

	if (LoadState == EBattleExperienceLoadState::Loaded)
	{
		LoadState = EBattleExperienceLoadState::Deactivating;

		NumExpectedPausers = INDEX_NONE;
		NumObservedPausers = 0;

		FGameFeatureDeactivatingContext Context(FSimpleDelegate::CreateUObject(this, &ThisClass::OnActionDeactivationCompleted));

		const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
		if (ExistingWorldContext)
		{
			Context.SetRequiredWorldContextHandle(ExistingWorldContext->ContextHandle);
		}
		
		auto DeactivateListOfActions = [&Context](const TArray<UGameFeatureAction*>& ActionList)
		{
			for (UGameFeatureAction* Action : ActionList)
			{
				if (Action)
				{
					Action->OnGameFeatureDeactivating(Context);
					Action->OnGameFeatureUnregistering();
				}
			}
		};

		DeactivateListOfActions(CurrentExperience->Actions);
		for (const TObjectPtr<UBattleExperienceActionSet>& ActionSet : CurrentExperience->ActionSets)
		{
			if (ActionSet != nullptr)
			{
				DeactivateListOfActions(ActionSet->Actions);
			}
		}

		NumExpectedPausers = Context.GetNumPausers();

		if (NumExpectedPausers > 0)
		{
			UE_LOG(LogBattle, Error, TEXT("Actions that have asynchronous deactivation aren't fully supported yet in Lyra experiences"));
		}

		if (NumExpectedPausers == NumObservedPausers)
		{
			OnAllActionsDeactivated();
		}
		
		
	}
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

	// ExpeirenceActionSet을 순회하며, BundleAssetList로 추가함
	for (const TObjectPtr<UBattleExperienceActionSet>& ActionSet : CurrentExperience->ActionSets)
	{
		if (ActionSet)
		{
			// BundleAssetList는 Bundle로 등록할 RootLayout인 PrimaryDataAsset을 추가하는 것임.
			BundleAssetList.Add(ActionSet->GetPrimaryAssetId());
		}
	}

	
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

	// 이전에 활성화된 GameFeature Plugin의 URL을 클리어 해줌.
	GameFeaturePluginURLs.Reset();

	// FeaturePluginList를 순회하면서, PluginName으로 PluginURL을 찾고 GameFeaturePluginURL에 등록해줌.
	auto CollectGameFeaturePluginURLs = [This = this](const UPrimaryDataAsset* Context, const TArray<FString>& FeaturePluginList)
	{
		for (const FString& PluginName : FeaturePluginList)
		{
			FString PluginURL;
			if (UGameFeaturesSubsystem::Get().GetPluginURLByName(PluginName, PluginURL))
			{
				This->GameFeaturePluginURLs.AddUnique(PluginURL);
			}
		}
	};

	// 등록해둔 GameFeaturesToEnable에 있는 Plugin만 활성화할 GameFeature Plugin 후보군으로 등록함.
	CollectGameFeaturePluginURLs(CurrentExperience, CurrentExperience->GameFeaturesToEnable);

	NumGameFeaturePluginsLoading = GameFeaturePluginURLs.Num();
	if (NumGameFeaturePluginsLoading)
	{
		LoadState = EBattleExperienceLoadState::LoadingGameFeature;
		for (const FString& PluginURL : GameFeaturePluginURLs)
		{
			// 매 Plugin이 로딩 및 활성화된 이후 OnGameFeaturePluginLoadComplete 콜백 함수를 실행하도록 함.
			UGameFeaturesSubsystem::Get().LoadAndActivateGameFeaturePlugin(PluginURL, FGameFeaturePluginLoadComplete::CreateUObject(this, &ThisClass::OnGameFeaturePluginLoadComplete));
		}
	}
	else
	{
		OnExperienceFullLoadCompleted();	
	}
	
}

void UBattleExperienceManagerComponent::OnGameFeaturePluginLoadComplete(const UE::GameFeatures::FResult& Result)
{
	// GameFeaturePlugin이 로딩되고 활성화됐을때 실행
	NumGameFeaturePluginsLoading--;
	if (NumGameFeaturePluginsLoading == 0)
	{
		// GameFeaturePlugin 로딩이 끝나면 기존의 Loaded대로 진행함.
		OnExperienceFullLoadCompleted();
	}
}

void UBattleExperienceManagerComponent::OnExperienceFullLoadCompleted()
{
	// After GameFeatureAction Load && Activation
	check(LoadState != EBattleExperienceLoadState::Loaded);
	
	// Start GameFeature Action Activation
	{
		LoadState = EBattleExperienceLoadState::ExecutingActions;

		FGameFeatureActivatingContext Context;
		{
			const FWorldContext* ExistingWorldContext = GEngine->GetWorldContextFromWorld(GetWorld());
			if (ExistingWorldContext)
			{
				Context.SetRequiredWorldContextHandle(ExistingWorldContext->ContextHandle);
			}
		}

		auto ActivateListOfActions = [&Context](const TArray<UGameFeatureAction*>& ActionList)
		{
			for (UGameFeatureAction* Action : ActionList)
			{
				// explicit GameFeatureAction state Set Registering => Loading => Activating
				if (Action)
				{
					Action->OnGameFeatureRegistering();
					Action->OnGameFeatureLoading();
					Action->OnGameFeatureActivating(Context);
				}
			}
		};
		
		ActivateListOfActions(CurrentExperience->Actions);

		for (const TObjectPtr<UBattleExperienceActionSet>& ActionSet : CurrentExperience->ActionSets)
		{
			ActivateListOfActions(ActionSet->Actions);
		}
	}
	
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

void UBattleExperienceManagerComponent::OnActionDeactivationCompleted()
{
	check(IsInGameThread());
	++NumObservedPausers;

	if (NumObservedPausers == NumExpectedPausers)
	{
		OnAllActionsDeactivated();
	}
}

void UBattleExperienceManagerComponent::OnAllActionsDeactivated()
{
	LoadState = EBattleExperienceLoadState::Unloaded;
	CurrentExperience = nullptr;
}
