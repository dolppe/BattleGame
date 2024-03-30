#include "BattleHUD.h"

#include "Components/GameFrameworkComponentManager.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleHUD)

ABattleHUD::ABattleHUD(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ABattleHUD::PreInitializeComponents()
{
	Super::PreInitializeComponents();


	UGameFrameworkComponentManager::AddGameFrameworkComponentReceiver(this);
}

void ABattleHUD::BeginPlay()
{
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, UGameFrameworkComponentManager::NAME_GameActorReady);
	Super::BeginPlay();
}

void ABattleHUD::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	UGameFrameworkComponentManager::RemoveGameFrameworkComponentReceiver(this);
	Super::EndPlay(EndPlayReason);	
}
