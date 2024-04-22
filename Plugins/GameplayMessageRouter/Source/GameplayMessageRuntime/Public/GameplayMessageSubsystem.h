#pragma once

#include "GameplayMessageTypes2.h"
#include "GameplayTagContainer.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameplayMessageSubsystem.generated.h"


class UAsyncAction_ListenForGameplayMessage;
class UGameplayMessageSubsystem;

GAMEPLAYMESSAGERUNTIME_API DECLARE_LOG_CATEGORY_EXTERN(LogGameplayMessageSubsystem, Log, All);

/*
 *	등록된 Message Listener를 지우기 위한 Handle
 *
 */

USTRUCT(BlueprintType)
struct GAMEPLAYMESSAGERUNTIME_API FGameplayMessageListenerHandle
{
public:
	GENERATED_BODY()

	FGameplayMessageListenerHandle() {}

	void Unregister();

	bool IsValid() const {return ID !=0;}

private:

	FGameplayMessageListenerHandle(UGameplayMessageSubsystem* InSubsystem, FGameplayTag InChannel, int32 InID)
	: Subsystem(InSubsystem), Channel(InChannel), ID(InID)
	{}
	
	UPROPERTY(Transient)
	TWeakObjectPtr<UGameplayMessageSubsystem> Subsystem;

	UPROPERTY(Transient)
	FGameplayTag Channel;

	UPROPERTY(Transient)
	int32 ID = 0;

	FDelegateHandle StateClearedHandle;

	friend UGameplayMessageSubsystem;
	
	
};

/*
 *	등록된 리스너 하나에 대한 데이터 단위.
 */

USTRUCT()
struct FGameplayMessageListenerData
{
	GENERATED_BODY()

	TFunction<void(FGameplayTag, const UScriptStruct*, const void*)> ReceivedCallback;

	int32 HandleID;
	EGameplayMessageMatch MatchType;

	TWeakObjectPtr<const UScriptStruct> ListenerStructType = nullptr;
	bool bHadValidType = false;	
	
};

/*
 *	서로 커플링 없이 메세지 이벤트 생성 및 구독 가능하게 함.
 *	멀티 리스너에 대해서 순서 보장 안됨.
 *
 */

UCLASS()
class GAMEPLAYMESSAGERUNTIME_API UGameplayMessageSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	friend UAsyncAction_ListenForGameplayMessage;
	
public:

	static UGameplayMessageSubsystem& Get(const UObject* WorldContextObject);

	static bool HasInstance(const UObject* WorldContextObject);

	virtual void Deinitialize() override;

	// StructType을 전달하지 않는 경우 추출하여 BroadCast 진행.
	template <typename FMessageStructType>
	void BroadcastMessage(FGameplayTag Channel, const FMessageStructType& Message)
	{
		const UScriptStruct* StructType = TBaseStructure<FMessageStructType>::Get();
		BroadCastMessageInternal(Channel, StructType, &Message);
	}

	// 리스너를 등록할 때, StructType을 저장해두어야 추후에 오는 Data가 제대로 왔는지 체크할 수 있음.
	// 또한, 해당 데이터는 void* 타입으로 오기 때문에 이를 캐스팅하여 Callback 함수를 진행할 수 있게 람다를 만들어둠.
	template <typename FMessageStructType>
	FGameplayMessageListenerHandle RegisterListener(FGameplayTag Channel, TFunction<void(FGameplayTag, const FMessageStructType&)>&& Callback, EGameplayMessageMatch MatchType = EGameplayMessageMatch::ExactMatch)
	{
		auto ThunkCallback = [InnterCallback = MoveTemp(Callback)] (FGameplayTag ActualTag, const UScriptStruct* SenderStructType, const void* SenderPayload)
		{
			InnterCallback(ActualTag, *reinterpret_cast<const FMessageStructType>(SenderPayload));	
		};

		const UScriptStruct* StructType = TBaseStructure<FMessageStructType>::Get();
		return RegisterListenerInternal(Channel, ThunkCallback, StructType, MatchType);
	}

	void UnregisterListener(FGameplayMessageListenerHandle Handle);
	
protected:

	/*
	 *	CustomThunk를 함수 지정자에 넣었기에, UHT에서 Thunk 함수를 만들지 않음. 직접 만들어줘야함.
	 *	이는 execK2_함수이름 형태로 함수를 만들면 이에 적용됨.
	 *	추가적으로 메타데이터에 CustomStructureParam을 통해 지정한 파라미터는 와일드카드처럼 어떠한 형태든 될 수 있음.
	 *	
	 */
	UFUNCTION(BlueprintCallable, CustomThunk, Category=Messaging, meta=(CustomStructureParam="Message", AllowAbstract="false", DisplayName="Broadcast Message"))
	void K2_BroadcastMessage(FGameplayTag Channel, const int32& Message);

	DECLARE_FUNCTION(execK2_BroadcastMessage);
		
private:

	/*
	 * 중요하게 봐야하는 것은 UScriptStruct임.
	 * 어떤 StructType이 와도 대응할 수 있도록 Payload 데이터를 void *로 변환하여 전송함.
	 * 이는 결국 Listener가 초기에 원했던 Struct 타입이 제대로 와야 void *을 다시 원래대로 변환했을 때 문제가 생기지 않음.
	 * 따라서 등록할 때도 원하는 타입에 대해 ScriptStruct로 저장해둠.
	 * Broadcast를 진행할 때도 전송하는 데이터의 StructType을 저장해둠.
	 * 전송이 되면, StructType을 체크하고, void *를 원래의 StructType으로 변환하는 과정을 거침. 안전장치 느낌.
	 */
	
	void BroadcastMessageInternal(FGameplayTag Channel, const UScriptStruct* StructType, const void* MessageBytes);

	FGameplayMessageListenerHandle RegisterListenerInternal(
		FGameplayTag Channel,
		TFunction<void(FGameplayTag, const UScriptStruct*, const void*)>&& Callback,
		const UScriptStruct* StructType,
		EGameplayMessageMatch MatchType);
	
	void UnregisterListenerInternal(FGameplayTag Channel, int32 HandleID);

	
private:

	struct FChannelListenerList
	{
		TArray<FGameplayMessageListenerData> Listeners;
		int32 HandleID = 0;
	};

	TMap<FGameplayTag, FChannelListenerList> ListenerMap;
	
};
