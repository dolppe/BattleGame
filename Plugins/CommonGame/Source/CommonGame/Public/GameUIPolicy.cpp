#include "GameUIPolicy.h"

#include "CommonLocalPlayer.h"
#include "PrimaryGameLayout.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameUIPolicy)

UPrimaryGameLayout* UGameUIPolicy::GetRootLayout(const UCommonLocalPlayer* LocalPlayer) const
{
	const FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer);
	return LayoutInfo ? LayoutInfo->RootLayout : nullptr;
}

TSubclassOf<UPrimaryGameLayout> UGameUIPolicy::GetLayoutWidgetClass(UCommonLocalPlayer* LocalPlayer)
{
	return LayOutClass.LoadSynchronous();
}

void UGameUIPolicy::CreateLayoutWidget(UCommonLocalPlayer* LocalPlayer)
{
	// PC가 있을 때 LayoutWidget을 생성 가능.
	if (APlayerController* PlayerController = LocalPlayer->GetPlayerController(GetWorld()))
	{
		TSubclassOf<UPrimaryGameLayout> LayoutWidgetClass = GetLayoutWidgetClass(LocalPlayer);
		if (ensure(LayoutWidgetClass) && !LayoutWidgetClass->HasAnyClassFlags(CLASS_Abstract))
		{
			UPrimaryGameLayout* NewLayoutObject = CreateWidget<UPrimaryGameLayout>(PlayerController, LayoutWidgetClass);

			RootViewportLayouts.Emplace(LocalPlayer, NewLayoutObject, true);

			AddLayoutToViewport(LocalPlayer, NewLayoutObject);
		}
	}
}

void UGameUIPolicy::AddLayoutToViewport(UCommonLocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout)
{
	// CommonUserWidget에 PlayerContext를 설정
	Layout->SetPlayerContext(FLocalPlayerContext(LocalPlayer));

	// 해당 Layout에 우선순위를 높게 설정함 (1000)
	// Widget을 붙임.
	Layout->AddToPlayerScreen(1000);
	
}

void UGameUIPolicy::RemoveLayoutFromViewport(UCommonLocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout)
{
	// UCommonUserWidget의 SlateWidget을 가져와서 Parent와 연결고리를 끊어둠.
	TWeakPtr<SWidget> LayoutSlateWidget = Layout->GetCachedWidget();
	if (LayoutSlateWidget.IsValid())
	{
		Layout->RemoveFromParent();
	}
}

void UGameUIPolicy::NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer)
{
	// PlayerController가 업데이트되면 GameLayout을 업데이트해주기 위해 Delegate를 추가함
	LocalPlayer->OnPlayerControllerSet.AddWeakLambda(this, [this](UCommonLocalPlayer* LocalPlayer, APlayerController* PlayerController)
	{
		// 우선 추가된 Player가 있으면 제거
		NotifyPlayerRemoved(LocalPlayer);

		// RootViewportLayouts를 순회하면서 검색
		// FRootViewportLayoutInfo의 operator== 정의가 있기 때문에 FindByKey로 찾는 것이 가능.
		if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
		{
			// Layout만 업데이트 해줌
			AddLayoutToViewport(LocalPlayer, LayoutInfo->RootLayout);
			LayoutInfo->bAddedToViewport = true;
		}
		else
		{
			// Layout을 생성하고 활성화함
			CreateLayoutWidget(LocalPlayer);
		}
	});

	if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		AddLayoutToViewport(LocalPlayer, LayoutInfo->RootLayout);
		LayoutInfo->bAddedToViewport = true;
	}
	else
	{
		CreateLayoutWidget(LocalPlayer);
	}
	
}

void UGameUIPolicy::NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer)
{
	if (FRootViewportLayoutInfo* LayoutInfo = RootViewportLayouts.FindByKey(LocalPlayer))
	{
		RemoveLayoutFromViewport(LocalPlayer, LayoutInfo->RootLayout);

		// 비활성화 확인 용으로 false 처리하는 것.
		LayoutInfo->bAddedToViewport = false;
	}		
}

void UGameUIPolicy::NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer)
{
	NotifyPlayerRemoved(LocalPlayer);

	// Player가 Destroy되므로, OnPlayerControllerSet에서 제거
	LocalPlayer->OnPlayerControllerSet.RemoveAll(this);

	// RootViewportLayouts에서도 제거
	const int32 LayoutInfoIdx = RootViewportLayouts.IndexOfByKey(LocalPlayer);
	if (LayoutInfoIdx != INDEX_NONE)
	{
		// 만약 PrimaryGameLayout이 있으면, Viewport에서도 제거
		UPrimaryGameLayout* Layout = RootViewportLayouts[LayoutInfoIdx].RootLayout;
		RootViewportLayouts.RemoveAt(LayoutInfoIdx);
		RemoveLayoutFromViewport(LocalPlayer, Layout);
	}
}
