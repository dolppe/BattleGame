#pragma once

#include "Containers/Array.h"
#include "GameFeatureStateChangeObserver.h"
#include "GameFeaturesProjectPolicies.h"
#include "UObject/Object.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "BattleGameFeaturePolicy.generated.h"

/*
 *	UBattleGameplayFeaturePolicy는 GameFeature Plugin이 Memory에 로딩되거나 활성화하는 것을 관리하는 StateMachine을 모니터링함.
 *	옵저버를 등록하고 해제하고 관리하는 것.
 */

UCLASS()
class UBattleGameplayFeaturePolicy : public UDefaultGameFeaturesProjectPolicies
{
	GENERATED_BODY()
public:
	UBattleGameplayFeaturePolicy(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void InitGameFeatureManager() override;
	virtual void ShutdownGameFeatureManager() override;

	// 옵저버로서 등록된 객체를 관리 (UBattleGameFeature_AddGameplayCuePaths이 하나의 예시임)
	UPROPERTY(Transient)
	TArray<TObjectPtr<UObject>> Observers;
	
};


/*
 *	GameFeature Plugin이 Register / UnRegister 단계에서 GameplayCuePath가 등록되어 GameplayCueManager가 관련 GameplayCue 에셋을 스캔할 수 있어야함
 *	이를 위해 IGameFeatureStateChangeObserver를 활용하여 옵저버 패턴으로 이를 구현
 *	 - GameplayFeaturePolicy를 통해 추가적으로 구현
 * - GameFeatureAction에도 Registering / Unregistering이 있긴 하지만, 이는 직접 호출해야 의미가 있음.
 */

UCLASS()
class UBattleGameFeature_AddGameplayCuePaths : public UObject, public IGameFeatureStateChangeObserver
{
	GENERATED_BODY()
public:

	// IGameFeatureStateChangeObserver Interface
	virtual void OnGameFeatureRegistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL) override;
	virtual void OnGameFeatureUnregistering(const UGameFeatureData* GameFeatureData, const FString& PluginName, const FString& PluginURL) override;
	
};


