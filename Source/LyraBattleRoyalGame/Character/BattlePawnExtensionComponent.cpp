#include "BattlePawnExtensionComponent.h"

#include "BattlePawnData.h"
#include "Components/GameFrameworkComponentManager.h"
#include "LyraBattleRoyalGame/BattleGameplayTags.h"
#include "LyraBattleRoyalGame/BattleLogChannels.h"
#include "LyraBattleRoyalGame/AbilitySystem/BattleAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattlePawnExtensionComponent)

// Feature Name은 어차피 Component 단위로 사용되기 때문에 Component를 빼고 네이밍.
const FName UBattlePawnExtensionComponent::NAME_ActorFeatureName("PawnExtension");

UBattlePawnExtensionComponent::UBattlePawnExtensionComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UBattlePawnExtensionComponent::OnRegister()
{
	Super::OnRegister();

	{
		if (!GetPawn<APawn>())
		{
			UE_LOG(LogBattle, Error, TEXT("this component has been added to a BP whose base class is not a Pawn!"));
		}
	}

	// GameFrameworkComponentManager에 InitState 사용을 위해 등록 진행
	RegisterInitStateFeature();
		
}

void UBattlePawnExtensionComponent::BeginPlay()
{
	Super::BeginPlay();

	// 해당 Actor의 원하는 FeatureComponent의 InitState 상태 변화에 델리게이트를 걸어두는 것 (변화되면 OnActorInitStateChanged가 호출)
	// FeatureName에 NAME_None을 넣으면 Actor에 등록된 모든 FeatureComponent의 InitState를 관찰하겠다는 것.
	BindOnActorInitStateChanged(NAME_None, FGameplayTag(),false);

	/*
	 * InitState_Spawned로 상태 변화 시도
	 *  - TryToChangeInitState는 아래와 같이 진행
	 *		1. CanChangeInitState => 상태 변화 가능성 유무 판단
	 *		2. HandleChangeInitState => FeatureComponent의 내부 상태 변환
	 *		3. BindOnActorInitStateChanged => Bind된 Delegate를 조건에 맞게 호출해 줌
	 *		 - PawnExtensionComponent의 경우 모든 Actor의 상태 변화에 따라 OnActorInitStateChanged가 호출됨
	 */
	ensure(TryToChangeInitState(FBattleGameplayTags::Get().InitState_Spawned));

	// 해당 함수는 재정의 할 것임
	// - ForceUpdateIniState와 같은 느낌임
	// - 현재 상테를 강제 업데이트 진행 => 당연히 조건은 체크함 ( CanChangeInitState, HandleChangeInitSTate 둘다 진행)
	CheckDefaultInitialization();
	
}

void UBattlePawnExtensionComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	Super::EndPlay(EndPlayReason);
}

// Bind 해둔 InitState의 상태 변화에 따라 호출됨.
// PawnExtensionComponent는 Actor의 모든 FeatureComponent에 대해서 해둠.
void UBattlePawnExtensionComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	// PawnExtensionComponent는 다른 Feature Component들의 상태가 DataAvailable인지 관찰해서 Sync를 맞춤 (CanChangedInitState)
	// 따라서 자신이 아닌 다른 Feature Component의 상태가 바뀌었을 때 DataAvailable이면 CheckDefaultInitialization을 호출하여 상태를 업데이트함
	if (Params.FeatureName != NAME_ActorFeatureName)
	{
		// 다른 FeatureComponent의 InitState가 만약 DataAvailable로 변경되면, 자신의 InitState를 업데이트 할 수 있는 지 체크하고 업데이트함.
		const FBattleGameplayTags& InitTags = FBattleGameplayTags::Get();
		if (Params.FeatureState == InitTags.InitState_DataAvailable)
		{
			CheckDefaultInitialization();
		}
	}
}

// InitState를 바꿀 수 있는지 체크하는 것.
bool UBattlePawnExtensionComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager,
	FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	APawn* Pawn = GetPawn<APawn>();
	const FBattleGameplayTags& InitTags = FBattleGameplayTags::Get();

	// None => InitState_Spawned
	// Spawned가 되는 것은 조건 없이 가능.
	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		if (Pawn)
		{
			return true;
		}
		
	}

	// InitState_Spawnd -> InitState_DataAvailable
	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		// PawnData가 설정 돼야 넘어갈 수 있음.
		// 해당 PawnData는 GameMode에서 Pawn이 생성될 때 PawnExtensionComponent의 PawnData를 설정하도록 할 것임
		if (!PawnData)
		{
			return false;
		}
		
		// 초기에는는 컨트롤러 세팅이 안되어 있는게 맞음
		// 아직 Possess가 안되어 있을 때 => bIsLocallyControlled가 ture가 되기 때문에 바로 DataAvailable로 변경됨.
		// 이는 예외처리 같은 느낌으로 있는 코드인듯.
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		if (bIsLocallyControlled)
		{
			if (!GetController<AController>())
			{
				return false;
			}
		}
		return true;
	}
	//  InitState_DataAvailable -> InitState_DataInitialized
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// Actor에 바인드 된 모든 Feature Component들이 DataAvailable 상태일때, True임.
		// PawnExtensionComponent가 InitState_DataInitialized이 됐다는 것은 다른 Component들도 최소 DataAvailable 상태라는 것.
		return Manager->HaveAllFeaturesReachedInitState(Pawn, InitTags.InitState_DataAvailable);
	}
	//  InitState_DataInitialized -> InitState_GameplayReady
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}
	
	// Current, Desired가 선형적으로 진행되지 않으면 false
	return false;
}

// 강제 업데이트 느낌.
void UBattlePawnExtensionComponent::CheckDefaultInitialization()
{
	// PawnExtensionComponent는 Feature Component들의 초기화를 관장함. 따라서 다른 Feature Component들의 CheckDefaultInitialization를 다 호출하도록 함.
	// IGameFrameworkInitStateInterface가 제공하는데, Actor에 바인딩된 Feature Component들이 CheckDefaultInitialization을 호출해주도록 한다. 
	CheckDefaultInitializationForImplementers();

	const FBattleGameplayTags& InitTags = FBattleGameplayTags::Get();

	static const TArray<FGameplayTag> StateChain = {InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady};

	// CanChangeInitState와 HandleChangeIniState 그리고 ChangeFeatrueInitState 호출을 통한 OnActorInitStateChange Delegate 호출까지 진행
	// 가능할 때 까지 상태를 계속 업데이트 함
	// InitState는 선형적으로 변화하기 때문에 가능
	// 업데이트가 멈춘다는 것은 다른 Component 혹은 다른 Actor의 세팅이나 진행이 필요한 것. (조건 덜 충족)
	ContinueInitStateChain(StateChain);
}

void UBattlePawnExtensionComponent::SetPawnData(const UBattlePawnData* InPawnData)
{
	// Pawn에 대해 Authority가 없으면 SetPawn을 안함.
	APawn* Pawn = GetPawnChecked<APawn>();
	if (Pawn->GetLocalRole() != ROLE_Authority)
	{
		return;
	}

	if (PawnData)
	{
		return;
	}
	
	PawnData = InPawnData;

	// 처음에는 어차피 CurrentState가 None이기 때문에 업데이트가 되지 않음.
	// PawnData가 업데이트 됐으니 InitState => Spawned -> DataAvailable로 업데이트 할 수 있도록 호출
	CheckDefaultInitialization();
}

void UBattlePawnExtensionComponent::SetupPlayerInputComponent()
{
	CheckDefaultInitialization();
}

void UBattlePawnExtensionComponent::InitializeAbilitySystem(UBattleAbilitySystemComponent* InASC, AActor* InOwnerActor)
{
	check (InASC && InOwnerActor);

	if (AbilitySystemComponent == InASC)
	{
		return;
	}
	if (AbilitySystemComponent)
	{
		UnInitializeAbilitySystem();
	}

	APawn* Pawn = GetPawnChecked<APawn>();
	AActor* ExistingAvatar = InASC->GetAvatarActor();
	check(!ExistingAvatar);

	AbilitySystemComponent = InASC;
	AbilitySystemComponent->InitAbilityActorInfo(InOwnerActor, Pawn);

	if (ensure(PawnData))
	{
		InASC->SetTagRelationshipMapping(PawnData->TagRelationshipMapping);
	}
	
	OnAbilitySystemInitialized.Broadcast();
	
}

void UBattlePawnExtensionComponent::UnInitializeAbilitySystem()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	if (AbilitySystemComponent->GetAvatarActor() == GetOwner())
	{
		OnAbilitySystemUninitialized.Broadcast();
	}
	
	AbilitySystemComponent = nullptr;
}

void UBattlePawnExtensionComponent::OnAbilitySystemInitialized_RegisterAndCall(
	FSimpleMulticastDelegate::FDelegate Delegate)
{
	if (!OnAbilitySystemInitialized.IsBoundToObject(Delegate.GetUObject()))
	{
		OnAbilitySystemInitialized.Add(Delegate);
	}

	if (AbilitySystemComponent)
	{
		Delegate.Execute();
	}
}

void UBattlePawnExtensionComponent::OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate Delegate)
{
	if (!OnAbilitySystemUninitialized.IsBoundToObject(Delegate.GetUObject()))
	{
		OnAbilitySystemUninitialized.Add(Delegate);
	}
}
