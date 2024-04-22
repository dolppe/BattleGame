#pragma once

#include "GameplayTagContainer.h"
#include "Templates/SharedPointer.h"
#include "Subsystems/WorldSubsystem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UIExtensionSystem.generated.h"

UENUM()
enum class EUIExtensionPointMatch : uint8
{
	// A.B는 A.B.C를 허용 x
	ExactMatch,

	// A.B는 A.B.C를 허용 o 
	PartialMatch,	
};


struct FUIExtension : TSharedFromThis<FUIExtension>
{
	// UIExtension Widget의 Point Tag임. (SlotTag)
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
	bool IsValid() const { return DataPtr.IsValid(); }
	
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

template <>
struct TStructOpsTypeTraits<FUIExtensionHandle> : public TStructOpsTypeTraitsBase2<FUIExtensionHandle>
{
	enum
	{
		WithCopy = true,
		WithIdenticalViaEquality = true,
	};
};

UENUM(BlueprintType)
enum class EUIExtensionAction : uint8
{
	Added,
	Removed,
};

USTRUCT(BlueprintType)
struct FUIExtensionRequest
{
	GENERATED_BODY()
public:
	// UIExtensionPoint와 연동될 Extension
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FUIExtensionHandle ExtensionHandle;
	
	// Extension의 Slot GameplayTag
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag ExtensionPointTag;

	// WidgetClass로 FUIExtenion과 같음.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UObject> Data = nullptr;

	// FUIExtension의 ContextObject를 전달 받음 (UCommonLocalPlayer, UModularPlayerState, ... == UIExtension을 실행한 Instigator)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UObject> ContextObject = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 Priority = INDEX_NONE;
};

DECLARE_DELEGATE_TwoParams(FExtendExtensionPointDelegate, EUIExtensionAction Action, const FUIExtensionRequest& Request);


struct FUIExtensionPoint : TSharedFromThis<FUIExtensionPoint>
{
public:

	// Extension과 ExtensionPoint가 매치되는지 확인.
	bool DoesExtensionPassContract(const FUIExtension* Extension) const;
	
	// UIExtension의 Slot GameplayTag
	FGameplayTag ExtensionPointTag;

	// UIExtension을 생성/제거한 주체
	TWeakObjectPtr<UObject> ContextObject;

	// UIExtensionPointWidget에 허용된 WidgetClass => UIExtensionPointWidget's DataClasses
	TArray<UClass*> AllowedDataClasses;

	// Widget을 ExtensionPointWidget에 연결하기 위한 Callback 함수
	FExtendExtensionPointDelegate Callback;
	EUIExtensionPointMatch ExtensionPointTagMatchType = EUIExtensionPointMatch::ExactMatch;
	
};

USTRUCT()
struct UIEXTENSION_API FUIExtensionPointHandle
{
	GENERATED_BODY()
public:
	FUIExtensionPointHandle() {}
	FUIExtensionPointHandle(UUIExtensionSubsystem* InExtensionSource, const TSharedPtr<FUIExtensionPoint>& InDataPtr)
		: ExtensionSource(InExtensionSource), DataPtr(InDataPtr)
	{}

	void Unregister();
	bool IsValid() const { return DataPtr.IsValid(); }
	bool operator==(const FUIExtensionPointHandle& Other) const { return DataPtr == Other.DataPtr; }
	bool operator!=(const FUIExtensionPointHandle& Other) const { return !operator==(Other); }
	
	TWeakObjectPtr<UUIExtensionSubsystem> ExtensionSource;
	TSharedPtr<FUIExtensionPoint> DataPtr;
	
};

UCLASS()
class UIEXTENSION_API UUIExtensionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:

	void UnregisterExtension(const FUIExtensionHandle& ExtensionHandle);
	FUIExtensionHandle RegisterExtensionAsWidgetForContext(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, TSubclassOf<UUserWidget> WidgetClass, int32 Priority);
	FUIExtensionHandle RegisterExtensionAsData(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, UObject* Data, int32 Priority);

	void UnregisterExtensionPoint(const FUIExtensionPointHandle& ExtensionPointHandle);
	FUIExtensionPointHandle RegisterExtensionPointForContext(const FGameplayTag& ExtensionPointTag, UObject* ContextObject, EUIExtensionPointMatch ExtensionPointTagMatchType, const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDelegate ExtensionCallback);
	FUIExtensionPointHandle RegisterExtensionPoint(const FGameplayTag& ExtensionPointTag, EUIExtensionPointMatch ExtensionPointTagMatchType, const TArray<UClass*>& AllowedDataClasses, FExtendExtensionPointDelegate ExtensionCallback);

	FUIExtensionRequest CreateExtensionRequest(const TSharedPtr<FUIExtension>& Extension);

protected:
	
	// ExtensionPoint -Broadcast => Extensions [ExtensionPoint가 추가/제거 됐을 때, Extensions에게 알림]
	void NotifyExtensionPointOfExtensions(TSharedPtr<FUIExtensionPoint>& ExtensionPoint);
	// Extension - Broadcast => ExtensionPoints [Extension이 추가/제거 됐을 때, ExtensionPoints에 알림]
	void NotifyExtensionPointsOfExtension(EUIExtensionAction Action, TSharedPtr<FUIExtension>& Extension);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Extension", meta = (DisplayName = "Register Extension (Widget For Context)"))
	FUIExtensionHandle K2_RegisterExtensionAsWidgetForContext(FGameplayTag ExtensionPointTag, TSubclassOf<UUserWidget> WidgetClass, UObject* ContextObject, int32 Priority = -1);
	

	
private:
	
	// GameplayTag(Slot) -- FUIExtension(WidgetClass)
	typedef TArray<TSharedPtr<FUIExtension>> FExtensionList;
	TMap<FGameplayTag, FExtensionList> ExtensionMap;

	// GameplayTag(Slot) -- FUIExtensionPoint(WidgetClassWithContext)
	typedef TArray<TSharedPtr<FUIExtensionPoint>> FExtensionPointList;
	TMap<FGameplayTag, FExtensionPointList> ExtensionPointMap;
	
};

UCLASS()
class UIEXTENSION_API UUIExtensionHandleFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UUIExtensionHandleFunctions() { }

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Extension")
	static void Unregister(UPARAM(ref) FUIExtensionHandle& Handle);

	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "UI Extension")
	static bool IsValid(UPARAM(ref) FUIExtensionHandle& Handle);
};
