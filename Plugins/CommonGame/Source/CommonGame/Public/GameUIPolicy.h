#pragma once

#include "UObject/Object.h"
#include "UObject/UObjectGlobals.h"
#include "GameUIPolicy.generated.h"

class UPrimaryGameLayout;

USTRUCT()
struct FRootViewportLayoutInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(Transient)
	TObjectPtr<ULocalPlayer> LocalPlayer = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UPrimaryGameLayout> RootLayout = nullptr;

	UPROPERTY(Transient)
	bool bAddedToViewport = false;
	
	bool operator==(const ULocalPlayer* OtherLocalPlayer) const {return LocalPlayer== OtherLocalPlayer;}

	FRootViewportLayoutInfo() {}
	FRootViewportLayoutInfo(ULocalPlayer* InLocalPlayer, UPrimaryGameLayout* InRootLayout, bool bIsInViewport)
		: LocalPlayer(InLocalPlayer)
		, RootLayout(InRootLayout)
		, bAddedToViewport(bIsInViewport)
	{}
	
};


UCLASS(Abstract, Blueprintable)
class COMMONGAME_API UGameUIPolicy : public UObject
{
	GENERATED_BODY()
public:

	UPrimaryGameLayout* GetRootLayout(const UCommonLocalPlayer* LocalPlayer) const;
	
	TSubclassOf<UPrimaryGameLayout> GetLayoutWidgetClass(UCommonLocalPlayer* LocalPlayer);
	void CreateLayoutWidget(UCommonLocalPlayer* LocalPlayer);
	
	void AddLayoutToViewport(UCommonLocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);
	void RemoveLayoutFromViewport(UCommonLocalPlayer* LocalPlayer, UPrimaryGameLayout* Layout);

	void NotifyPlayerAdded(UCommonLocalPlayer* LocalPlayer);
	void NotifyPlayerRemoved(UCommonLocalPlayer* LocalPlayer);
	void NotifyPlayerDestroyed(UCommonLocalPlayer* LocalPlayer);

	
	// LocalPlayer에 바인딩된 UI의 Layout으로 생각하면 됨.
	// UIPolicy에 해당하는 Default UI Layout 느낌.
	UPROPERTY(EditAnywhere)
	TSoftClassPtr<UPrimaryGameLayout> LayOutClass;

	// 보통 싱글게임에서는 LocalPlayer, PrimaryGameLayout이 하나만 있음.
	// 그러나 멀티플레이의 경우 복수개가 가능함 ex) 리플레이
	UPROPERTY(Transient)
	TArray<FRootViewportLayoutInfo> RootViewportLayouts;
	
};


