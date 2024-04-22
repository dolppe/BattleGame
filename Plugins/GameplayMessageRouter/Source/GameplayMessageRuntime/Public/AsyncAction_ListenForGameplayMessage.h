#pragma once

#include "GameplayMessageSubsystem.h"
#include "GameplayMessageTypes2.h"
#include "Delegates/Delegate.h"
#include "Engine/CancellableAsyncAction.h"
#include "GameplayTagContainer.h"
#include "AsyncAction_ListenForGameplayMessage.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsyncGameplayMessageDelegate, UAsyncAction_ListenForGameplayMessage*, ProxyObject, FGameplayTag, ActualChannel);

// HasDedicatedAsyncNode를 통해 자동으로 생성해주는 UK2_Node의 생성을 막음.
// AsyncAction Node를 생성하는 Factory Function은 직접 호출하는 것이 아닌 K2Node_AsyncAction_ListenForGameplayMessage를 통해서 커스텀 노드로 제공함.
UCLASS(BlueprintType, meta=(HasDedicatedAsyncNode))
class GAMEPLAYMESSAGERUNTIME_API UAsyncAction_ListenForGameplayMessage : public UCancellableAsyncAction
{
	GENERATED_BODY()
public:
	/*
	 * UAsyncAction_ListenForGameplayMessage를 생성하는 Factory Function임
	 * 그러나 이를 직접 이용하면, 전송되는 Struct의 변수들을 제대로 이용하기 힘듦. (BPGraph상에서)
	 * 따라서 BlueprintInternalUseOnly 지정자를 추가하여 이를 직접 호출할 수 없도록 함. 이는 (K2Node_AsyncAction_ListenForGameplayMessage를 통해서 호출됨.)
	 * 추가적으로 UAsyncAction에서 자동으로 만들어주는 UK2_Node 또한 만들지 않도록 설정함. (HasDedicatedAsyncNode를 활용하여)
	 * => K2Node_AsyncAction_ListenForGameplayMessage를 통해서 커스텀 Node를 만들고 해당 함수랑 연결할 것이기 때문임.
	*/
	UFUNCTION(BlueprintCallable, Category = Messaging, meta = (WorldContext = "WorldContextObject", BlueprintInternalUseOnly = "true"))
	static UAsyncAction_ListenForGameplayMessage* ListenForGameplayMessages(UObject* WorldContextObject, FGameplayTag Channel, UScriptStruct* PayloadType, EGameplayMessageMatch MatchType = EGameplayMessageMatch::ExactMatch);

	/*
	 * 전달받은 PayloadData(void*)를 맞는 Struct Type으로 변환시킴.
	 * Node에서 불리는 경우 void* => 맞는 Struct로 타입 캐스팅이 일어나지 않기 때문에
	 * 이를 진행해주기 위함.
	 * 따라서 이 또한 K2Node_AsyncAction 쪽에서 PayloadData가 전송된 후에 실행하도록 처리함.
	 */
	UFUNCTION(BlueprintCallable, CustomThunk, Category="Messaging", meta=(CustomStructureParam = "OutPayload"))
	bool GetPayload(UPARAM(ref) int& OutPayload);

	DECLARE_FUNCTION(execGetPayload);
	
	// 활성화 됐을 때 리스너 등록을 진행함
	virtual void Activate() override;
	virtual void SetReadyToDestroy() override;

public:
	// 메세지가 왔을 때 Broadcast됨. 
	UPROPERTY(BlueprintAssignable)
	FAsyncGameplayMessageDelegate OnMessageReceived;

private:
	// 메세지가 도착했을 때 Payload 데이터를 저장하고 델리게이트를 Broadcast함.
	void HandleMessageReceived(FGameplayTag Channel, const UScriptStruct* StructType, const void* Payload);
	
private:

	// 받은 데이터
	const void* ReceivedMessagePayloadPtr = nullptr;

	TWeakObjectPtr<UWorld> WorldPtr;
	// 등록해둘 Tag
	FGameplayTag ChannelToRegister;
	// 전송받길 원하는 StructType
	TWeakObjectPtr<UScriptStruct> MessageStructType = nullptr;
	EGameplayMessageMatch MessageMatchType = EGameplayMessageMatch::ExactMatch;

	// Unregister를 위한 Handle 캐싱
	FGameplayMessageListenerHandle ListenerHandle;
	
};

