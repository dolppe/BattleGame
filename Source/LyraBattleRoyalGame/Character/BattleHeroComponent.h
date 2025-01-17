#pragma once

#include "GameplayAbilitySpecHandle.h"
#include "InputActionValue.h"
#include "Components/GameFrameworkInitStateInterface.h"
#include "Components/PawnComponent.h"
#include "LyraBattleRoyalGame/Input/BattleInputConfig.h"

#include "BattleHeroComponent.generated.h"

class UChaosVehicleMovementComponent;
class ABattleWheeledVehiclePawn;
class IBattleInteractActorInterface;
class UBattleInputConfig;
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

	UFUNCTION(BlueprintPure, Category="Battle|Hero")
	static UBattleHeroComponent* FindHeroComponent(const AActor* Actor) {return (Actor ? Actor->FindComponentByClass<UBattleHeroComponent>() : nullptr);}
	
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
	void SetAbilityCameraMode(TSubclassOf<UBattleCameraMode> CameraMode, const FGameplayAbilitySpecHandle& OwningSpecHandle);

	void ClearAbilityCameraMode(const FGameplayAbilitySpecHandle& OwningSpecHandle);
	
	// Input
	void InitilizePlayerInput(UInputComponent* PlayerInputComponent);
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	void Input_Crouch(const FInputActionValue& InputActionValue);
	void Input_Interact(const FInputActionValue& InputActionValue);
	void Input_AbilityInputTagPressed(FGameplayTag InputTag);
	void Input_AbilityInputTagReleased(FGameplayTag InputTag);

	bool IsReadyToBindInputs() const;
	void AdditionalInputConfig(const UBattleInputConfig* InputConfig);
	void RemoveAdditionalInputConfig(const UBattleInputConfig* InputConfig);

	void SetInteraction(IBattleInteractActorInterface* InInteraction);

	UFUNCTION(BlueprintCallable)
	void SetIsVehicle(bool bInIsVehicle)
	{
		bIsVehicle = bInIsVehicle;
	}

	UFUNCTION(BlueprintCallable)
	bool GetIsVehicle()
	{
		return bIsVehicle;
	}
	
	UFUNCTION(BlueprintCallable)
	void SetCurrentVehicle(UChaosVehicleMovementComponent* InCurrentVehicle)
	{
		CurrentVehicleMovement = InCurrentVehicle;
	}

	UFUNCTION(BlueprintCallable)
	UChaosVehicleMovementComponent* GetCurrentVehicle()
	{
		return CurrentVehicleMovement;
	}
	
	
protected:
	
	UPROPERTY(EditAnywhere)
	TArray<FBattleMappableConfigPair> DefaultInputConfigs;

	UPROPERTY()
	TSubclassOf<UBattleCameraMode> AbilityCameraMode;

	FGameplayAbilitySpecHandle AbilityCameraModeOwningSpecHandle;

	bool bReadyToBindInputs;

	IBattleInteractActorInterface* CurrentInteract;
	
	UChaosVehicleMovementComponent* CurrentVehicleMovement;
	bool bIsVehicle = false;

	float PrevSteeringInput = 0.0f;
	float PrevThrottleInput = 0.0f;
	float PrevBrakeInput = 0.0f;
	
};
