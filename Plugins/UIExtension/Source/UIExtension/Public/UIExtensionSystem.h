#pragma once

#include "GameplayTagContainer.h"
#include "Templates/SharedPointer.h"
#include "Subsystems/WorldSubsystem.h"
#include "UIExtensionSystem.generated.h"

struct FUIExtension : TSharedFromThis<FUIExtension>
{
	// UIExtension Widget의 Point Tag임.
	FGameplayTag ExtensionPointTag;

	// WidgetClass를 가지고 있으며, UUIExtensionSubsystem에서 AddReferencedObjects에서 GC가 되는 것을 막음.
	UObject* Data = nullptr;

	// 보통 LocalPlayer로 설정됨.
	TWeakObjectPtr<UObject> ContextObject;

	int32 Priority = INDEX_NONE;
};


USTRUCT(BlueprintType)
struct UIEXTENSION_API FUIExtensionHandle
{
	GENERATED_BODY()
public:
	FUIExtensionHandle() {}
	FUIExtensionHandle(UUIExtensionSubsystem* InExtensionSource, const TSharedPtr<FUIExtension>& InDataPtr)
		: ExtensionSource(InExtensionSource)
		, DataPtr(InDataPtr)
	{}

	void Unregister();
	bool IsValid() const {return DataPtr.IsValid();}
	bool operator==(const FUIExtensionHandle& Other) const {return DataPtr==Other.DataPtr;}
	bool operator!=(const FUIExtensionHandle& Other) const {return !operator==(Other);}

	friend FORCEINLINE uint32 GetTypeHash(FUIExtensionHandle Handle)
	{
		return PointerHash(Handle.DataPtr.Get());
	}

	friend class UUIExtensionSubsystem;
	TWeakObjectPtr<UUIExtensionSubsystem> ExtensionSource;
	TSharedPtr<FUIExtension> DataPtr;
	
};


UCLASS()
class UIEXTENSION_API UUIExtensionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:

	
};


