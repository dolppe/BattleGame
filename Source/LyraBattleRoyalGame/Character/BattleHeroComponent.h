#pragma once

#include "InputActionValue.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"

#include "BattleHeroComponent.generated.h"

class UBattleCameraMode;
template<class TClass> class TSubclassOf;
struct FBattleMappableConfigPair;


//  카메라, 입력 등 플레이어가 제어하는 시스템의 초기화를 담당.
UCLASS(Blueprintable, Meta=(BlueprintSpawnableComponent))
class UBattleHeroComponent : public UPawnComponent, public IGameFrameworkInitStateInterface
{
	GENERATED_BODY()
public:
	UBattleHeroComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// GameFeature Name
	static const FName NAME_ActorFeatureName;

	// GameFrameworkComponentManager Extension Custom Event Name
	static const FName NAME_BindInputsNow;

	// PawnComponent Interface
	virtual void OnRegister() final;
	virtual void BeginPlay() final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) final;

	// IGameFrameworkInitStateInterface Interface
	virtual FName GetFeatureName() const final {return NAME_ActorFeatureName;}
	virtual void CheckDefaultInitialization() final;
	virtual void OnActorInitStateChanged(const FActorInitStateChangedParams& Params) final;
	virtual bool CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const final;
	virtual void HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) final;

	// Camera
	TSubclassOf<UBattleCameraMode> DetermineCameraMode() const;

	// Input
	void InitilizePlayerInput(UInputComponent* PlayerInputComponent);
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	UPROPERTY(EditAnywhere)
	TArray<FBattleMappableConfigPair> DefaultInputConfigs;
	
};
