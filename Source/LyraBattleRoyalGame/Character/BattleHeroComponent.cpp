#include "BattleHeroComponent.h"

#include "BattleCharacter.h"
#include "BattlePawnExtensionComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerMappableInputConfig.h"
#include "Components/GameFrameworkComponentManager.h"
#include "LyraBattleRoyalGame/BattleGameplayTags.h"
#include "LyraBattleRoyalGame/BattleLogChannels.h"
#include "LyraBattleRoyalGame/AbilitySystem/BattleAbilitySystemComponent.h"
#include "LyraBattleRoyalGame/Camera/BattleCameraComponent.h"
#include "LyraBattleRoyalGame/Camera/BattleCameraMode.h"
#include "LyraBattleRoyalGame/Input/BattleInputComponent.h"
#include "LyraBattleRoyalGame/Player/BattlePlayerState.h"
#include "LyraBattleRoyalGame/Input/BattleMappableConfigPair.h"
#include "LyraBattleRoyalGame/Player/BattlePlayerController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleHeroComponent)

const FName UBattleHeroComponent::NAME_ActorFeatureName("Hero");

const FName UBattleHeroComponent::NAME_BindInputsNow("BindInputsNow");

UBattleHeroComponent::UBattleHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	AbilityCameraMode = nullptr;
}

void UBattleHeroComponent::OnRegister()
{
	Super::OnRegister();
	{
		if (!GetPawn<APawn>())
		{
			UE_LOG(LogBattle, Error, TEXT("this component has been added to a BP whose base class is not a Pawn!"));
			return;
		}
	}

	// UGameFrameworkComponentManager에 InitState를 사용하는 GameFeature Component라고 등록하는 것.
	RegisterInitStateFeature();
}

void UBattleHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// PawnExtensionComponent가 다른 Component들의 초기화도 담당하기 때문에
	// PawnExtensionComponent의 InitState만 관찰하도록 함.
	BindOnActorInitStateChanged(UBattlePawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(),false);

	// None => InitState_Spawned
	ensure(TryToChangeInitState(FBattleGameplayTags::Get().InitState_Spawned));

	CheckDefaultInitialization();
}

void UBattleHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	Super::EndPlay(EndPlayReason);
}

// 연쇄적으로 자신의 InitState를 업데이트할 수 있으면 업데이트 하도록 하는 것 (None인 경우 동작x)
void UBattleHeroComponent::CheckDefaultInitialization()
{
	// Hero Feature는 PawnExtension Feature에 종속되어 있어서, CheckDefaultInitializationforImplemanters를 호출하지 않음
	// 다른 Component들의 InitState 업데이트를 진행하지 않음. (이는 오로지 PawnExtensionComponent에서만 관장하기 위하여 사용)
	const FBattleGameplayTags& InitTags = FBattleGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = {InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady};
	ContinueInitStateChain(StateChain);
}

void UBattleHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	const FBattleGameplayTags& InitTags = FBattleGameplayTags::Get();
	if (Params.FeatureName == UBattlePawnExtensionComponent::NAME_ActorFeatureName)
	{
		if (Params.FeatureState == InitTags.InitState_DataInitialized)
		{
			// PawnExtensionComponent이 DataInitilized 단계가 되면 그떄 HeroComponent도 InitState를 바꿀 수 있는지 체크
			// PawnExtensionComponent가 InitState_DataInitialized이 됐다는 것은 다른 Component도 다 최소 DataAvailable이라는 것임.
			CheckDefaultInitialization();
		}
	}
}

PRAGMA_DISABLE_OPTIMIZATION

bool UBattleHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	check(Manager);

	const FBattleGameplayTags& InitTags = FBattleGameplayTags::Get();
	APawn* Pawn = GetPawn<APawn>();
	ABattlePlayerState* BattlePS = GetPlayerState<ABattlePlayerState>();

	// None -> InitState_Spawned
	// Pawn이 존재 => Component이기에 오류만 없으면 무조건 존재.
	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		if (Pawn)
		{
			return true;
		}
	}
	
	// InitState_Spawned -> InitState_DataAvailable
	// PlayerState가 설정되어 있어야 넘어갈 수 있음.
	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		if (!BattlePS)
		{
			return false;
		}

		if (Pawn->IsLocallyControlled() && !Pawn->IsBotControlled())
		{
			ABattlePlayerController* BattlePC = GetController<ABattlePlayerController>();

			if (!Pawn->InputComponent || !BattlePC || !BattlePC->GetLocalPlayer())
			{
				return false;
			}
		}
		return true;
	}

	// PawnExtensionComponent가 DataInitialized 상태이면 모든 Feature가 최소 DataAvailable임
	// 즉, 다른 모든 Feature Component가 최소 DataAvailable 상태라는 것.
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		return BattlePS && Manager->HasFeatureReachedInitState(Pawn,UBattlePawnExtensionComponent::NAME_ActorFeatureName, InitTags.InitState_DataInitialized);
	}
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}
	
	return false;
	
}



// InitState 상태 변화가 진행될 때 호출하는 함수.
void UBattleHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState)
{
	const FBattleGameplayTags& InitTags = FBattleGameplayTags::Get();

	// DataAviliable -> DataInitilized 단계
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ABattlePlayerState* BattlePS = GetPlayerState<ABattlePlayerState>();
		// DataInitilized 단계로 넘어가려면 Pawn과 PlayerState가 설정되어 있어야함.
		if (!ensure(Pawn && BattlePS))
		{
			return;
		}

		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const UBattlePawnData* PawnData = nullptr;
		if (UBattlePawnExtensionComponent* PawnExtensionComponent = UBattlePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtensionComponent->GetPawnData<UBattlePawnData>();

			PawnExtensionComponent->InitializeAbilitySystem(BattlePS->GetBattleAbilitySystemComponent(), BattlePS);
		}

		if (ABattlePlayerController* BattlePC = GetController<ABattlePlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				InitilizePlayerInput(Pawn->InputComponent);
			}
		}
		
		if (bIsLocallyControlled && PawnData)
		{
			// 현재 Character에 Attach된 CameraComponent
			if (UBattleCameraComponent* CameraComponent = UBattleCameraComponent::FindCameraComponent(Pawn))
			{
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}
		
	}
	
}

PRAGMA_ENABLE_OPTIMIZATION

TSubclassOf<UBattleCameraMode> UBattleHeroComponent::DetermineCameraMode() const
{
	if (AbilityCameraMode)
	{
		return AbilityCameraMode;
	}
	
	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}

	if (UBattlePawnExtensionComponent* PawnExtComp = UBattlePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UBattlePawnData* PawnData = PawnExtComp->GetPawnData<UBattlePawnData>())
		{
			return PawnData->DefaultCameraMode;
		}
	}
	
	return nullptr;
}

void UBattleHeroComponent::SetAbilityCameraMode(TSubclassOf<UBattleCameraMode> CameraMode,
	const FGameplayAbilitySpecHandle& OwningSpecHandle)
{
	if (CameraMode)
	{
		AbilityCameraMode = CameraMode;
		AbilityCameraModeOwningSpecHandle = OwningSpecHandle;
	}
}

void UBattleHeroComponent::InitilizePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	// EnhancedInputLocalPlayerSubsystem에 매핑 컨텍스트을 초기화 시켜줌.
	Subsystem->ClearAllMappings();

	// PawnExtComp => PawnData => InputConfig 존재 판단.
	if (const UBattlePawnExtensionComponent* PawnExtComp = UBattlePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UBattlePawnData* PawnData = PawnExtComp->GetPawnData<UBattlePawnData>())
		{
			if (const UBattleInputConfig* InputConfig = PawnData->InputConfig)
			{
				const FBattleGameplayTags& GameplayTags = FBattleGameplayTags::Get();

				// HeroComponent에 있는 Input Mapping Context를 순회하며, 이를 EnhancedInputLocalPlayerSubsystem에 추가함.
				for (const FBattleMappableConfigPair& Pair : DefaultInputConfigs)
				{
					if (Pair.bShouldActivateAutomatically)
					{
						FModifyContextOptions Options = {};
						Options.bIgnoreAllPressedKeysUntilRelease = false;

						// 내부적으로 Input Mapping Context를 추가함.
						// 이를 먼저 추가해야 바인딩을 제대로 진행할 수 있음.
						Subsystem->AddPlayerMappableConfig(Pair.Config.LoadSynchronous(), Options);
					}
				}
				
				UBattleInputComponent* BattleIC = CastChecked<UBattleInputComponent>(PlayerInputComponent);
				{
					{
						TArray<uint32> BindHandles;
						BattleIC->BindAbilityActions(InputConfig, this, &ThisClass::Input_AbilityInputTagPressed, &ThisClass::Input_AbilityInputTagReleased, BindHandles);
					}
					
					// InputTag_Move, InputTag_Look_Mouse에 대해 각각 Input_Move, Input_LookMouse 멤버 함수를 바인딩함.
					// 바인딩을 진행하면, 이후 Input 이벤트에 따라 멤버 함수가 트리거됨.
					BattleIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this,&ThisClass::Input_Move, false);
					BattleIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this,&ThisClass::Input_LookMouse, false);
					BattleIC->BindNativeAction(InputConfig, GameplayTags.InputTag_Crouch, ETriggerEvent::Triggered, this,&ThisClass::Input_Crouch, false);
				}
				
			}
		}
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
	
}

void UBattleHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			// Left/Right -> x값에 들어있음.
			// MovementDirection은 현재 카메라의 RightVector를 의미함.
			// 월드 스페이스로의 변환을 의미함.
			// 캐릭터의 오른쪽 왼쪽을 구분하려면 카메라를 기준으로 월드 스페이스를 구하고, 해당 카메라의 우측 벡터를 가져오면됨.
			// 로컬 -> 월드로 전환하는 느낌.
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);

			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f)
		{
			// Foward를 적용하기 위해 Swizzle를 진행했음 (IMC)
			
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);

			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
	
}

void UBattleHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}
	const FVector2D Value = InputActionValue.Get<FVector2D>();
	if (Value.X != 0.0f)
	{
		// X에는 Yaw값이 있음
		// 카메라에 Yaw 적용.
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		// Y에는 Pitch 값.
		double AimInversionValue = -Value.Y;
		Pawn->AddControllerPitchInput(AimInversionValue);
	}
	
}

void UBattleHeroComponent::Input_Crouch(const FInputActionValue& InputActionValue)
{
	if (ABattleCharacter* Character = GetPawn<ABattleCharacter>())
	{
		Character->ToggleCrouch();
	}
	
}

PRAGMA_DISABLE_OPTIMIZATION

void UBattleHeroComponent::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const UBattlePawnExtensionComponent* PawnExtComp = UBattlePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (UBattleAbilitySystemComponent* BattleASC = PawnExtComp->GetAbilitySystemComponent())
			{
				BattleASC->AbilityInputTagPressed(InputTag);
			}
		}
	}
}

PRAGMA_ENABLE_OPTIMIZATION

void UBattleHeroComponent::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (const APawn* Pawn = GetPawn<APawn>())
	{
		if (const UBattlePawnExtensionComponent* PawnExtComp = UBattlePawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			if (UBattleAbilitySystemComponent* BattleASC = PawnExtComp->GetAbilitySystemComponent())
			{
				BattleASC->AbilityInputTagReleased(InputTag);
			}
		}
	}
}





