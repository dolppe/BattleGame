#include "BattleInputConfig.h"

#include "LyraBattleRoyalGame/BattleLogChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleInputConfig)

UBattleInputConfig::UBattleInputConfig(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const UInputAction* UBattleInputConfig::FindNativeInputActionForTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	// NativeInputActions를 순회하며, Input으로 들어온 InputTag가 있는지 체크함.
	// - 있으면 그에 따른 InputAction을 반환하고 없으면, nullptr을 반환함.
	// validation 느낌임.

	for (const FBattleInputAction& Action : NativeInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogBattle,Error, TEXT("Can't find NativeInputAction for InputTag [%s] on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
	
}

const UInputAction* UBattleInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag,
	bool bLogNotFound) const
{
	for (const FBattleInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && (Action.InputTag == InputTag))
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogBattle,Error, TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
	
}
