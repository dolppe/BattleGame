#pragma once

#include "BlueprintGraph/Classes/K2Node_AsyncAction.h"
#include "K2Node_AsyncAction_ListenForGameplayMessages.generated.h"


UCLASS()
class UK2Node_AsyncAction_ListenForGameplayMessages : public UK2Node_AsyncAction
{
	GENERATED_BODY()
private:


	/*
	 * UEdGraphNode interface
	 */
	virtual void PostReconstructNode() override;
	virtual void PinDefaultValueChanged(UEdGraphPin* ChangedPin) override;

	
	/*
	 * UK2Node interface
	 */
	
	// 이는 Blueprint Graph에서 K2Node를 만드는 함수임.
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;

	virtual void AllocateDefaultPins() override;

protected:
	
	/*
	 * UK2Node_BaseAsyncTask interface
	 */

	// AsyncAction 델리게이트를 다루기 위한 함수
	// 이를 통해 마지막 Output 핀 등을 조절할 수 있음.
	virtual bool HandleDelegates(const TArray<FBaseAsyncTaskHelper::FOutputPinAndLocalVariable>& VariableOutputs, UEdGraphPin* ProxyObjectPin, UEdGraphPin*& InOutLastThenPin, UEdGraph* SourceGraph, FKismetCompilerContext& CompilerContext) override;

private:

	bool HandlePayloadImplementation(
		FMulticastDelegateProperty* CurrentProperty,
		const FBaseAsyncTaskHelper::FOutputPinAndLocalVariable& ProxyObjectVar,
		const FBaseAsyncTaskHelper::FOutputPinAndLocalVariable& PayloadVar,
		const FBaseAsyncTaskHelper::FOutputPinAndLocalVariable& ActualChannelVar,
		UEdGraphPin*& InOutLastActivatedThenPin, UEdGraph* SourceGraph, FKismetCompilerContext& CompilerContext);

	// Make sure the output Payload wildcard matches the input PayloadType
	void RefreshOutputPayloadType();

	UEdGraphPin* GetPayloadPin() const;
	UEdGraphPin* GetPayloadTypePin() const;
	UEdGraphPin* GetOutputChannelPin() const;
};
