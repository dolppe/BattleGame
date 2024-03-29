#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "GameFeatureAction_WorldActionBase.h"
#include "GameplayTagContainer.h"
#include "UIExtensionSystem.h"
#include "GameFeatureAction_AddWidget.generated.h"

struct FComponentRequestHandle;

USTRUCT()
struct FBattleHUDLayoutRequest
{
	GENERATED_BODY()

	// UI의 레이아웃으로 CommonActivatableWidget을 사용
	UPROPERTY(EditAnywhere, Category=UI, meta=(AssetBundles="Client"))
	TSoftClassPtr<UCommonActivatableWidget> LayoutClass;

	// PrimaryGameLayout의 LayerID
	UPROPERTY(EditAnywhere, Category=UI)
	FGameplayTag LayerID;
	
};

USTRUCT()
struct FBattleHUDElementEntry
{
	GENERATED_BODY()

	// LayoutRequest에 올릴 대상이 되는 위젯 클래스
	UPROPERTY(EditAnywhere, Category=UI, meta=(AssetBundles="Client"))
	TSoftClassPtr<UUserWidget> WidgetClass;

	// SlotID는 LayoutReqeust에 올린 LayoutClass에 정의된 Slot에 대한 GameplayTag임.
	UPROPERTY(EditAnywhere, Category=UI)
	FGameplayTag SlotID;
};

UCLASS(meta=(DisplayName="Add Widgets"))
class UGameFeatureAction_AddWidgets final : public UGameFeatureAction_WorldActionBase
{
	GENERATED_BODY()
public:

	struct FPerContextData
	{
		TArray<TSharedPtr<FComponentRequestHandle>> ComponentRequests;
		TArray<TWeakObjectPtr<UCommonActivatableWidget>> LayoutsAdded;

		// HUDElement는 UIExtension으로 관리됨
		TArray<FUIExtensionHandle> ExtensionHandles;
	};

	
	

	// GameFeatureAction의 Add Remove 상태를 관리함. 
	TMap<FGameFeatureStateChangeContext, FPerContextData> ContextData; 
	
	// 형태를 정의하는 Layout, 해당 Layout에 올릴 Widget 객체로 구성됨.
	UPROPERTY(EditAnywhere, Category=UI)
	TArray<FBattleHUDLayoutRequest> Layout;

	UPROPERTY(EditAnywhere, Category=UI)
	TArray<FBattleHUDElementEntry> Widgets;
	
};
