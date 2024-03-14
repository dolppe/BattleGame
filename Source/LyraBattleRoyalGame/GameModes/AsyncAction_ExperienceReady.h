#pragma once

#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncAction_ExperienceReady.generated.h"

class AGameStateBase;
class UBattleExperienceDefinition;


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FExperienceReadyAsyncDelegate);

UCLASS()
class UAsyncAction_ExperienceReady : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UAsyncAction_ExperienceReady(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	// UAsyncAction_ExperienceReady를 생성하고 기다리는 BP 호출
	UFUNCTION(BlueprintCallable, meta=(WorldContext= "WorldContextObject"))
	static UAsyncAction_ExperienceReady* WaitForExperienceReady(UObject* WorldContextObject);
	
	virtual void Activate() override;

	// Experience가 로딩됐는지 확실하게 하기 위함.
	void Step1_HandleGameStateSet(AGameStateBase* GameState);
	void Step2_ListenToExperienceLoading(AGameStateBase* GameState);
	void Step3_HandleExperienceLoaded(const UBattleExperienceDefinition* CurrentExperience);
	void Step4_BroadcastReady();
	
	// BlueprintAssingnable은 BP 상에서 할당 가능한 변수로 정의하는 것
	UPROPERTY(BlueprintAssignable)
	FExperienceReadyAsyncDelegate OnReady;

	TWeakObjectPtr<UWorld> WorldPtr;
	
};


