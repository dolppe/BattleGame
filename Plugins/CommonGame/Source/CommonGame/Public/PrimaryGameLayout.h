#pragma once

#include "CommonUserWidget.h"
#include "GameplayTagContainer.h"
#include "PrimaryGameLayout.generated.h"


class UCommonActivatableWidgetContainerBase;

/*
 *	인게임에서 메인 UI의 레이아웃을 담당하는 UMG
 *	- PrimaryGameLayout은 플레이어당 하나씩 가지는 최상위 UI 레이아웃
 *	- 레이어 기반으로 UI를 관리함.
 */

UCLASS()
class COMMONGAME_API UPrimaryGameLayout : public UCommonUserWidget
{
	GENERATED_BODY()
public:

	UPrimaryGameLayout(const FObjectInitializer& ObjectInitializer);

	UCommonActivatableWidgetContainerBase* GetLayerWidget(FGameplayTag LayerName);

	// Layer를 추가하며, GameplayTag를 할당함.
	UFUNCTION(BlueprintCallable, Category="Layer")
	void RegisterLayer(FGameplayTag LayerTag, UCommonActivatableWidgetContainerBase* LayerWidget);

	// GameplayTag <-> CommonActivatableWidgetContainerBase
	UPROPERTY(Transient, meta=(Categories = "UI.Layer"))
	TMap<FGameplayTag, TObjectPtr<UCommonActivatableWidgetContainerBase>> Layers;
	
};

