#include "GameFeatureAction_AddWidget.h"

#include "CommonUIExtensions.h"
#include "Components/GameFrameworkComponentManager.h"
#include "LyraBattleRoyalGame/UI/BattleHUD.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFeatureAction_AddWidget)


void UGameFeatureAction_AddWidgets::AddWidgets(AActor* Actor, FPerContextData& ActiveData)
{
	ABattleHUD* HUD = CastChecked<ABattleHUD>(Actor);

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(HUD->GetOwningPlayerController()->Player))
	{
		for (const FBattleHUDLayoutRequest& Entry : Layout)
		{
			if (TSubclassOf<UCommonActivatableWidget> ConcreteWidgetClass = Entry.LayoutClass.Get())
			{
				ActiveData.LayoutsAdded.Add(UCommonUIExtensions::PushContentToLayer_ForPlayer(LocalPlayer, Entry.LayerID, ConcreteWidgetClass));
			}
		}

		// Widget을 순회하며 UIExtensionSubsystem의 Extension에 추가함.
		UUIExtensionSubsystem* ExtensionSubsystem = HUD->GetWorld()->GetSubsystem<UUIExtensionSubsystem>();
		for (const FBattleHUDElementEntry& Entry : Widgets)
		{
			ActiveData.ExtensionHandles.Add(ExtensionSubsystem->RegisterExtensionAsWidgetForContext(Entry.SlotID, LocalPlayer, Entry.WidgetClass.Get(),-1));
		}
		
	}


	
	
}

void UGameFeatureAction_AddWidgets::RemoveWidgets(AActor* Actor, FPerContextData& ActiveData)
{
	ABattleHUD* HUD = CastChecked<ABattleHUD>(Actor);

	for (TWeakObjectPtr<UCommonActivatableWidget>& AddedLayout : ActiveData.LayoutsAdded)
	{
		if (AddedLayout.IsValid())
		{
			AddedLayout->DeactivateWidget();
		}
	}
	ActiveData.LayoutsAdded.Reset();

	for (FUIExtensionHandle& Handle : ActiveData.ExtensionHandles)
	{
		Handle.Unregister();
	}
	ActiveData.ExtensionHandles.Reset();
}

void UGameFeatureAction_AddWidgets::Reset(FPerContextData& ActiveData)
{
	ActiveData.ComponentRequests.Empty();
	ActiveData.LayoutsAdded.Empty();

	for (FUIExtensionHandle& Handle : ActiveData.ExtensionHandles)
	{
		Handle.Unregister();
	}

	ActiveData.ExtensionHandles.Reset();
	
}

void UGameFeatureAction_AddWidgets::HandleActorExtension(AActor* Actor, FName EventName,
														 FGameFeatureStateChangeContext ChangeContext)
{
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

	if ((EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved) || (EventName ==UGameFrameworkComponentManager::NAME_ReceiverRemoved))
	{
		RemoveWidgets(Actor, ActiveData);
	}
	else if ((EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded) || (EventName == UGameFrameworkComponentManager::NAME_GameActorReady))
	{
		AddWidgets(Actor, ActiveData);
	}
	
}


void UGameFeatureAction_AddWidgets::AddToWorld(const FWorldContext& WorldContext,
	const FGameFeatureStateChangeContext& ChangeContext)
{
	UWorld* World = WorldContext.World();
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

	// GameFrameworkComponentManager를 가져올 GameInstance와 World는 GameWorld이어야 함.
	if ((GameInstance != nullptr) && (World != nullptr) && World->IsGameWorld())
	{
		if (UGameFrameworkComponentManager* ComponentManager = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{
			// Widget을 추가할 대상으로 ABattleHUD로 고정함
			// UGameFrameworkComponentManager의 리시버로 등록해둠.
			TSoftClassPtr<AActor> HUDActorClass = ABattleHUD::StaticClass();

			// GameFeatureAction_WroldBase와 마찬가지로 HandleActorExtension을 콜백으로 받음.
			TSharedPtr<FComponentRequestHandle> ExtensionRequestHandle = ComponentManager->AddExtensionHandler(
				HUDActorClass,
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandleActorExtension, ChangeContext));

			ActiveData.ComponentRequests.Add(ExtensionRequestHandle);
		}
		
	}
	
	
}

void UGameFeatureAction_AddWidgets::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);

	FPerContextData* ActiveData = ContextData.Find(Context);
	if (ensure(ActiveData))
	{
		Reset(*ActiveData);
	}
}

