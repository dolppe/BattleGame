#include "UIExtensionSystem.h"
#include "Blueprint/UserWidget.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(UIExtensionSystem)

void FUIExtensionHandle::Unregister()
{
	if (UUIExtensionSubsystem* ExtensionSourcePtr = ExtensionSource.Get())
	{
		ExtensionSource->UnregisterExtension(*this);
	}
}

void UUIExtensionSubsystem::UnregisterExtension(const FUIExtensionHandle& ExtensionHandle)
{
	if (ExtensionHandle.IsValid())
	{
		// 반드시 ExtensionHandle의 source가 UUIExtensionSubsystem과 같은지 확인 해야함
		checkf(ExtensionHandle.ExtensionSource == this, TEXT("Trying to unregister an extension"));

		TSharedPtr<FUIExtension> Extension = ExtensionHandle.DataPtr;

		if (FExtensionList* ListPtr = ExtensionMap.Find(Extension->ExtensionPointTag))
		{
			ListPtr->RemoveSwap(Extension);
			if (ListPtr->Num() == 0)
			{
				// Num() == 0이면 Map에서도 제거 진행
				ExtensionMap.Remove(Extension->ExtensionPointTag);
			}
		}
		
	}
}

FUIExtensionHandle UUIExtensionSubsystem::RegisterExtensionAsWidgetForContext(const FGameplayTag& ExtensionPointTag,
                                                                              UObject* ContextObject, TSubclassOf<UUserWidget> WidgetClass, int32 Priority)
{
	return RegisterExtensionAsData(ExtensionPointTag, ContextObject, WidgetClass, Priority);
}

FUIExtensionHandle UUIExtensionSubsystem::RegisterExtensionAsData(const FGameplayTag& ExtensionPointTag,
	UObject* ContextObject, UObject* Data, int32 Priority)
{
	// Slot Tag가 Invalid
	if (!ExtensionPointTag.IsValid())
	{
		return FUIExtensionHandle();
	}

	// WidgetClass가 없음
	if (!Data)
	{
		return FUIExtensionHandle(); 
	}

	FExtensionList& List = ExtensionMap.FindOrAdd(ExtensionPointTag);

	TSharedPtr<FUIExtension>& Entry = List.Add_GetRef(MakeShared<FUIExtension>());
	Entry->ExtensionPointTag = ExtensionPointTag;
	Entry->ContextObject = ContextObject;
	Entry->Data = Data;
	Entry->Priority = Priority;

	return FUIExtensionHandle(this, Entry);
	
}
