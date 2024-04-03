#include "BattleActivatableWidget.h"
#include "CommonInputModeTypes.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleActivatableWidget)

UBattleActivatableWidget::UBattleActivatableWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

TOptional<FUIInputConfig> UBattleActivatableWidget::GetDesiredInputConfig() const
{
	// WidgetInputMode에 따라서 InputConfig를 설정해줌
	// InputMode에 따라 Game/Menu 혹은 둘다로 정할 수 잇음.
	// ex) Menu로 들어갔을 때 Game에 Input을 더이상 보내고 싶지 않을 때 좋음.
	switch (InputConfig)
	{
	case EBattleWidgetInputMode::GameAndMenu:
		return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode);
	case EBattleWidgetInputMode::Game:
		return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode);
	case EBattleWidgetInputMode::Menu:
		return FUIInputConfig(ECommonInputMode::Menu, GameMouseCaptureMode);
	case EBattleWidgetInputMode::Default:
	default:
		return TOptional<FUIInputConfig>();
	}
	
	return Super::GetDesiredInputConfig();
}
