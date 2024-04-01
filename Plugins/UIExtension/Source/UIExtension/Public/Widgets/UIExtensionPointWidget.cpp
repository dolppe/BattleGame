#include "UIExtensionPointWidget.h"
#include "CommonLocalPlayer.h"
#include "GameFramework/PlayerState.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(UIExtensionPointWidget)

#define LOCTEXT_NAMESPACE "UIExtension"

UUIExtensionPointWidget::UUIExtensionPointWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UUIExtensionPointWidget::ResetExtensionPoint()
{
	// UDynamicExntrBoxBase::ResetInternal을 호출
	ResetInternal();

	// UserWidget은 알아서 GC 될 거이니 컨테이너만 Reset
	ExtensionMapping.Reset();

	// ExtensionPointHandle을 순회하며 UIExtensionSystem에서 제거
	for (FUIExtensionPointHandle& Handle : ExtensionPointHandles)
	{
		Handle.Unregister();
	}
	ExtensionPointHandles.Reset();
}

void UUIExtensionPointWidget::RegisterExtensionPoint()
{
	if (UUIExtensionSubsystem* ExtensionSubsystem = GetWorld()->GetSubsystem<UUIExtensionSubsystem>())
	{
		TArray<UClass*> AllowedDataClasses;
		AllowedDataClasses.Add(UUserWidget::StaticClass());
		AllowedDataClasses.Append(DataClasses);

		// nullptr용 (ContextObject) ExtensionPoint 생성
		ExtensionPointHandles.Add(ExtensionSubsystem->RegisterExtensionPoint(
			ExtensionPointTag, ExtensionPointTagMatch, AllowedDataClasses,
			FExtendExtensionPointDelegate::CreateUObject(this, &ThisClass::OnAddOrRemoveExtension)
		));

		// LocalPlayer용 (ContextObject) ExtensionPoint 생성
		ExtensionPointHandles.Add(ExtensionSubsystem->RegisterExtensionPointForContext(
			ExtensionPointTag, GetOwningLocalPlayer(),ExtensionPointTagMatch, AllowedDataClasses,
			FExtendExtensionPointDelegate::CreateUObject(this, &ThisClass::OnAddOrRemoveExtension)
		));
		
	}
}

void UUIExtensionPointWidget::RegisterExtensionPointForPlayerState(UCommonLocalPlayer* LocalPlayer,
	APlayerState* PlayerState)
{
	if (UUIExtensionSubsystem* ExtensionSubsystem = GetWorld()->GetSubsystem<UUIExtensionSubsystem>())
	{
		TArray<UClass*> AllowedDataClasses;
		AllowedDataClasses.Add(UUserWidget::StaticClass());
		AllowedDataClasses.Append(DataClasses);
		
		// PlayerState용 (ContextObject) ExtensionPoint 생성
		ExtensionPointHandles.Add(ExtensionSubsystem->RegisterExtensionPointForContext(
			ExtensionPointTag, PlayerState,ExtensionPointTagMatch, AllowedDataClasses,
			FExtendExtensionPointDelegate::CreateUObject(this, &ThisClass::OnAddOrRemoveExtension)
		));
		
	}
}

void UUIExtensionPointWidget::OnAddOrRemoveExtension(EUIExtensionAction Action, const FUIExtensionRequest& Request)
{

	if (Action == EUIExtensionAction::Added)
	{
		UObject* Data = Request.Data;

		TSubclassOf<UUserWidget> WidgetClass(Cast<UClass>(Data));
		if (WidgetClass)
		{
			// WidgetClass를 활용하여 CreateEntryInternal(DynamicEntryBoxBase에 있음)를 호출해서 Child Widget 생성
			UUserWidget* Widget = CreateEntryInternal(WidgetClass);

			// 제거할 경우 Tracking하기 위해 ExtensionMapping에 추가
			ExtensionMapping.Add(Request.ExtensionHandle,Widget);
		}
		
	}
	else
	{
		// ExtensionMapping을 활용하여 안전하게 Remove해버림
		if (UUserWidget* Extension = ExtensionMapping.FindRef(Request.ExtensionHandle))
		{
			RemoveEntryInternal(Extension);
			ExtensionMapping.Remove(Request.ExtensionHandle);
		}
	}
	
}

TSharedRef<SWidget> UUIExtensionPointWidget::RebuildWidget()
{
	// 실제 인게임 렌더링 될때 실행
	if (!IsDesignTime() && ExtensionPointTag.IsValid())
	{
		// UIExtensionPointWidget 내부 UDynamicEntryBoxBase::Child를 모두 제거
		ResetExtensionPoint();

		// 다시 ExtensionPointWidget을 등록하기 위한 델리게이트, 핸들 추가
		RegisterExtensionPoint();

		// PlayerState 설정에 대한 델리게이트를 통해 ExtensionPointWidget 추가를 위한 호출 확보
		FDelegateHandle Handle = GetOwningLocalPlayer<UCommonLocalPlayer>()->CallAndRegister_OnPlayerStateSet(
			UCommonLocalPlayer::FPlayerStateSetDelegate::FDelegate::CreateUObject(this, &ThisClass::RegisterExtensionPointForPlayerState)
		);
		
	}

	// UMG Editor에서 실행될때 실행되는 구간 
	if (IsDesignTime())
	{
		auto GetExtensionPointText = [this]()
		{
			// ExtensionPoint
			// {ExtensionPointTag}
			// 위와 같은 형태로 표현
			return FText::Format(LOCTEXT("DesignTime_ExtensionPointLabel", "ExtensionPoint\n{0}"), FText::FromName(ExtensionPointTag.GetTagName()));
		};

		// 그냥 Editor HUD임 SOverlay
		TSharedRef<SOverlay> MessageBox = SNew(SOverlay);
		MessageBox->AddSlot()
			.Padding(0.5f)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
					.Justification(ETextJustify::Center)
					.Text_Lambda(GetExtensionPointText)
			];
		return MessageBox;
	}
	else
	{
		return Super::RebuildWidget();
	}	
}
