#include "BattleGameInstance.h"
#include "LyraBattleRoyalGame/BattleGameplayTags.h"
#include "Components/GameFrameworkComponentManager.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleGameInstance)

UBattleGameInstance::UBattleGameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UBattleGameInstance::Init()
{
	Super::Init();
	
	UGameFrameworkComponentManager* ComponentManager = GetSubsystem<UGameFrameworkComponentManager>(this);

	// 사용할 GameplayTag를 등록함.
	if (ensure(ComponentManager))
	{
		const FBattleGameplayTags& GameplayTags = FBattleGameplayTags::Get();

		// InitState 관련 Tag 등록.
		ComponentManager->RegisterInitState(GameplayTags.InitState_Spawned, false, FGameplayTag());
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataAvailable, false, GameplayTags.InitState_Spawned);
		ComponentManager->RegisterInitState(GameplayTags.InitState_DataInitialized, false, GameplayTags.InitState_DataAvailable);
		ComponentManager->RegisterInitState(GameplayTags.InitState_GameplayReady, false, GameplayTags.InitState_DataInitialized);
	}
	
}

void UBattleGameInstance::Shutdown()
{
	Super::Shutdown();
}
