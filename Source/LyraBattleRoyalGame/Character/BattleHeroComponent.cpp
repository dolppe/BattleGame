#include "BattleHeroComponent.h"

#include "BattlePawnExtensionComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "LyraBattleRoyalGame/BattleGameplayTags.h"
#include "LyraBattleRoyalGame/BattleLogChannels.h"
#include "LyraBattleRoyalGame/Player/BattlePlayerState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleHeroComponent)

const FName UBattleHeroComponent::NAME_ActorFeatureName("Hero");

UBattleHeroComponent::UBattleHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
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

	RegisterInitStateFeature();
	
}

void UBattleHeroComponent::BeginPlay()
{
	Super::BeginPlay();

	// PawnExtensionComponent가 다른 Component들의 초기화도 담당하기 때문에
	// PawnExtensionComponent의 InitState만 관찰하도록 함.
	BindOnActorInitStateChanged(UBattlePawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(),false);

	ensure(TryToChangeInitState(FBattleGameplayTags::Get().InitState_Spawned));

	CheckDefaultInitialization();
}

void UBattleHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UnregisterInitStateFeature();
	Super::EndPlay(EndPlayReason);
}

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

bool UBattleHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState,
	FGameplayTag DesiredState) const
{
	check(Manager);

	const FBattleGameplayTags& InitTags = FBattleGameplayTags::Get();
	APawn* Pawn = GetPawn<APawn>();
	ABattlePlayerState* ClonePS = GetPlayerState<ABattlePlayerState>();

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
		if (!ClonePS)
		{
			return false;
		}
		return true;
	}

	// PawnExtensionComponent가 DataInitialized 상태이면 모든 Feature가 최소 DataAvailable임
	// 즉, 다른 모든 Feature Component가 최소 DataAvailable 상태라는 것.
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		return ClonePS && Manager->HasFeatureReachedInitState(Pawn,UBattlePawnExtensionComponent::NAME_ActorFeatureName, InitTags.InitState_DataInitialized);
	}
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		return true;
	}
	
	return false;
	
}

// InitState 상태 변화를 위해 호출하는 함수
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
	}
	
}
