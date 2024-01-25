#pragma once
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"

#include "BattlePawnExtensionComponent.generated.h"

class UBattlePawnData;

// 초기화 순서 등 전반을 조정하는 컴포넌트
// 다른 Feature Component들의 초기화 순서를 전체적으로 관장함.
UCLASS()
class UBattlePawnExtensionComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
public:
	UBattlePawnExtensionComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// FeatureName 정의
	static const FName NAME_ActorFeatureName;

	
	// UPawnComponent Interface
	virtual void OnRegister() final;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) final;

	// IGameFrameworkInitStateInterface Interface
	virtual FName GetFeatureName() const override { return NAME_ActorFeatureName;}
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	virtual void CheckDefaultInitialization() final;

	// Member Methods
	template <class T>
	const T* GetPawnData() const {return Cast<T>(PawnData);}
	void SetPawnData(const UBattlePawnData* InPawnData);
	static UBattlePawnExtensionComponent* FindPawnExtensionComponent(const AActor* Actor) {return (Actor ? Actor->FindComponentByClass<UBattlePawnExtensionComponent>() : nullptr);}
	void SetupPlayerInputComponent();
	
	UPROPERTY(EditInstanceOnly, Category= "Battle|Pawn")
	TObjectPtr<const UBattlePawnData> PawnData;

	
};
