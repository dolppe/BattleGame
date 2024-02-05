#pragma once

#include "EnhancedInputComponent.h"
#include "BattleInputConfig.h"
#include "BattleInputComponent.generated.h"


UCLASS()
class UBattleInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
public:
	UBattleInputComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	template <class UserClass, typename FuncType>
	void BindNativeAction(const UBattleInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound);

	template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const UBattleInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);
};

template <class UserClass, typename FuncType>
void UBattleInputComponent::BindNativeAction(const UBattleInputConfig* InputConfig, const FGameplayTag& InputTag,
	ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound)
{
	check(InputConfig);

	// InputConfig는 활성화 가능한 InputAction을 담고 있음.
	// 만약 InputConfig에 없는 InputAction을 바인딩 시키면, nullptr을 반환해서 바인딩을 막음
	if (const UInputAction* IA = InputConfig->FindNativeInputActionForTag(InputTag, bLogIfNotFound))
	{
		BindAction(IA, TriggerEvent, Object, Func);
	}
	
}

template <class UserClass, typename PressedFuncType, typename ReleasedFuncType>
void UBattleInputComponent::BindAbilityActions(const UBattleInputConfig* InputConfig, UserClass* Object,
	PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles)
{
	check(InputConfig);

	// AblityAction에 대해서는 그냥 모든 InputAction에 다 바인딩 시킴
	for (const FBattleInputAction& Action : InputConfig->AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag.IsValid())
		{
			if (PressedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Triggered, Object, PressedFunc, Action.InputTag).GetHandle());
			}
			if (ReleasedFunc)
			{
				BindHandles.Add(BindAction(Action.InputAction, ETriggerEvent::Completed, Object, ReleasedFunc, Action.InputTag).GetHandle());
			}
		}
	}

	
}

