#pragma once

#include "GameplayTagContainer.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "GameplayMessageTypes2.generated.h"

UENUM(BlueprintType)
enum class EGameplayMessageMatch : uint8
{
	ExactMatch,
	PartialMatch
};

/*
 *	Struct used to specify advanced behavior when registering a listener for gameplay messages
 */

template<typename FMessageStructType>
struct FGameplayMessageListenerParams
{
	EGameplayMessageMatch MatchType = EGameplayMessageMatch::ExactMatch;

	TFunction<void(FGameplayTag, const FMessageStructType&)> OnMessageReceivedCallback;

	/** Helper to bind weak member function to OnMessageReceivedCallback */
	template<typename TOwner = UObject>
	void SetMessageReceivedCallback(TOwner* Object, void(TOwner::* Function)(FGameplayTag, const FMessageStructType&))
	{
		TWeakObjectPtr<TOwner> WeakObject(Object);
		OnMessageReceivedCallback = [WeakObject, Function] (FGameplayTag Channel, const FMessageStructType& Payload)
		{
			if (TOwner* StrongObject = WeakObject.Get())
			{
				(StrongObject->*Function)(Channel, Payload);
			}
		};
	}
};

