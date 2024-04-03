#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "BattleActivatableWidget.generated.h"


// Input 처리 방식 정의
UENUM(BlueprintType)
enum class EBattleWidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu,
};

UCLASS(Abstract, Blueprintable)
class UBattleActivatableWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()
public:
	UBattleActivatableWidget(const FObjectInitializer& ObjectInitializer);

	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;

	// Input 처리 방식
	UPROPERTY(EditDefaultsOnly, Category=Input)
	EBattleWidgetInputMode InputConfig = EBattleWidgetInputMode::Default;

	// Mouse 처리 방식
	UPROPERTY(EditDefaultsOnly, Category=Input)
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently;
	
};
