#pragma once
#include "GameFeatureAction_WorldActionBase.h"

#include "GameFeatureAction_AddInputConfig.generated.h"

struct FBattleMappableConfigPair;
struct FComponentRequestHandle;

UCLASS()
class UGameFeatureAction_AddInputConfig : public UGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()
public:

	virtual void OnGameFeatureActivating(FGameFeatureActivatingContext& Context) override;
	virtual void OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context) override;
	
	virtual void AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext) override;

private:
	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ExtensionRequestHandles;
		TArray<TWeakObjectPtr<APawn>> PawnsAddedTo;
	};

	// Call when GameFeatureState is Changed or GameFramewrokComponentManager start ExtensionEvent
	void HandlePawnExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext);

	void AddInputConfig(APawn* Pawn, FPerContextData& ActiveData);
	void RemoveInputConfig(APawn* Pawn, FPerContextData& ActiveData);
	void Reset(FPerContextData& ActiveData);

	// World GameFeatureStateContext -> (Extension Request, Pawn)
	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData;

	// PlayerMappableInputConfig
	UPROPERTY(EditAnywhere)
	TArray<FBattleMappableConfigPair> InputConfigs;
	
};
