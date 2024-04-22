#include "K2Node_AsyncAction_ListenForGameplayMessages.h"

#include "AsyncAction_ListenForGameplayMessage.h"
#include "BlueprintActionDatabaseRegistrar.h"
#include "BlueprintFunctionNodeSpawner.h"
#include "K2Node_CallFunction.h"
#include "K2Node_TemporaryVariable.h"
#include "KismetCompiler.h"
#include "K2Node_AssignmentStatement.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(K2Node_AsyncAction_ListenForGameplayMessages)

namespace UK2Node_AsyncAction_ListenForGameplayMessagesHelper
{
	static FName ActualChannelPinName = "ActualChannel";
	static FName PayloadPinName = "Payload";
	static FName PayloadTypePinName = "PayloadType";
	static FName DelegateProxyPinName = "ProxyObject";
};


void UK2Node_AsyncAction_ListenForGameplayMessages::PostReconstructNode()
{
	Super::PostReconstructNode();

	RefreshOutputPayloadType();
}

void UK2Node_AsyncAction_ListenForGameplayMessages::PinDefaultValueChanged(UEdGraphPin* ChangedPin)
{
	if (ChangedPin == GetPayloadTypePin())
	{
		if (ChangedPin->LinkedTo.Num() == 0)
		{
			RefreshOutputPayloadType();
		}
	}
}

// 이 함수를 통해 Node와 AsyncAction_ListenForGameplayMessage의 함수를 연결함.
void UK2Node_AsyncAction_ListenForGameplayMessages::GetMenuActions(
	FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	// 노드에 Function의 값을 적용하기 위한 함수.
	// 밑의 RegisterClassFactoryActions를 통해 추후에 불림.
	// NodeSpwner의 CustomNodeDelegate를 통해 호출되며, 그 전에 신규 노드와 연결할 함수를 찾아둠. (RegisterClassFactoryActions)
	struct GetMenuActions_Utils
	{
		static void SetNodeFunc(UEdGraphNode* NewNode, bool /*bIsTemplateNode*/,TWeakObjectPtr<UFunction> FunctionPtr)
		{
			UK2Node_AsyncAction_ListenForGameplayMessages* AsyncTaskNode = CastChecked<UK2Node_AsyncAction_ListenForGameplayMessages>(NewNode);
			if (FunctionPtr.IsValid())
			{
				UFunction* Func = FunctionPtr.Get();
				FObjectProperty* ReturnProp = CastFieldChecked<FObjectProperty>(Func->GetReturnProperty());

				AsyncTaskNode->ProxyFactoryFunctionName = Func->GetFName();
				AsyncTaskNode->ProxyFactoryClass		= Func->GetOuterUClass();
				AsyncTaskNode->ProxyClass				= ReturnProp->PropertyClass;
			}
		}
	};

	// 노드의 Class 역할로 현재 K2Node 클래스를 가져옴.
	UClass* NodeClass = GetClass();
	// 이는 template 타입으로 전달한 UAsyncAction_ListenForGameplayMessage의 내부를 확인함.
	// 해당 클래스의 FactoryType의 Function (자기 자신 클래스를 생성하는)을 찾음.
	// 찾으면, 매개변수로 전달한 람다함수를 실행하며, 해당 Function을 넘겨줌. => FactoryFunc
	ActionRegistrar.RegisterClassFactoryActions<UAsyncAction_ListenForGameplayMessage>(FBlueprintActionDatabaseRegistrar::FMakeFuncSpawnerDelegate::CreateLambda([NodeClass](const UFunction* FactoryFunc)->UBlueprintNodeSpawner*
	{
		// NodeSpawner를 생성하는 함수이며, Funtion을 연결하며 함수의 지정자, 메타데이터 등을 통해 올바른 노드를 생성하고, 해당 노드의 필요한 값들을 채움.
		// 채우는 값은 전달한 Function을 통해서 채우며, 해당 노드의 메뉴 카테고리, 이름, 툴팁, 키워드 같은 것들을 가져와서 설정함. 
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintFunctionNodeSpawner::Create(FactoryFunc);
		check (NodeSpawner != nullptr);
		NodeSpawner->NodeClass = NodeClass;

		TWeakObjectPtr<UFunction> FunctionPtr = MakeWeakObjectPtr(const_cast<UFunction*>(FactoryFunc));
		NodeSpawner->CustomizeNodeDelegate = UBlueprintNodeSpawner::FCustomizeNodeDelegate::CreateStatic(GetMenuActions_Utils::SetNodeFunc, FunctionPtr);

		return NodeSpawner;
	}));
	
}

void UK2Node_AsyncAction_ListenForGameplayMessages::AllocateDefaultPins()
{
	Super::AllocateDefaultPins();

	// 델리게이트로 전송되는 ProxyObject는 어차피 AsyncAction 스스로에 대한 것이므로
	// 어차피 AsyncAction 핀은 Node 특성상 제공되기 때문에 숨겨버림. (필요 없기 때문에)
	UEdGraphPin* DelegateProxyPin = FindPin(UK2Node_AsyncAction_ListenForGameplayMessagesHelper::DelegateProxyPinName);
	if (ensure(DelegateProxyPin))
	{
		DelegateProxyPin->bHidden = true;
	}

	// Payload 데이터에 대해서 Pin이 생성되도록 함
	// 어떤 Struct에 대해서도 대응할 수 있도록 Wildcard Pin을 생성함.
	// 이는 RefreshOutputPayloadType를 통해 설정됨.
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Wildcard, UK2Node_AsyncAction_ListenForGameplayMessagesHelper::PayloadPinName);
	
}

bool UK2Node_AsyncAction_ListenForGameplayMessages::HandleDelegates(
	const TArray<FBaseAsyncTaskHelper::FOutputPinAndLocalVariable>& VariableOutputs, UEdGraphPin* ProxyObjectPin,
	UEdGraphPin*& InOutLastThenPin, UEdGraph* SourceGraph, FKismetCompilerContext& CompilerContext)
{
	bool bIsErrorFree = true;

	// 델리게이트 관련 Output 핀은 3개 여야함
	// ProxyObject, Channel, Payload
	if (VariableOutputs.Num() != 3)
	{
		ensureMsgf(false, TEXT("UK2Node_AsyncAction_ListenForGameplayMessages::HandleDelegates - Variable output array not valid. Output delegates must only have the single proxy object output and than must have pin for payload."));
		return false;
	}

	for (TFieldIterator<FMulticastDelegateProperty> PropertyIt(ProxyClass); PropertyIt && bIsErrorFree; ++PropertyIt)
	{
		UEdGraphPin* LastActivatedThenPin = nullptr;
		bIsErrorFree &= FBaseAsyncTaskHelper::HandleDelegateImplementation(*PropertyIt, VariableOutputs, ProxyObjectPin, InOutLastThenPin, LastActivatedThenPin, this, SourceGraph, CompilerContext);

		bIsErrorFree &= HandlePayloadImplementation(*PropertyIt, VariableOutputs[0], VariableOutputs[2], VariableOutputs[1], LastActivatedThenPin, SourceGraph, CompilerContext);
	}

	return bIsErrorFree;
}

bool UK2Node_AsyncAction_ListenForGameplayMessages::HandlePayloadImplementation(
	FMulticastDelegateProperty* CurrentProperty, const FBaseAsyncTaskHelper::FOutputPinAndLocalVariable& ProxyObjectVar,
	const FBaseAsyncTaskHelper::FOutputPinAndLocalVariable& PayloadVar,
	const FBaseAsyncTaskHelper::FOutputPinAndLocalVariable& ActualChannelVar, UEdGraphPin*& InOutLastActivatedThenPin,
	UEdGraph* SourceGraph, FKismetCompilerContext& CompilerContext)
{
	bool bIsErrorFree = true;
	const UEdGraphPin* PayloadPin = GetPayloadPin();
	const UEdGraphSchema_K2* Schema = CompilerContext.GetSchema();

	check(CurrentProperty && SourceGraph && Schema);

	const FEdGraphPinType& PinType = PayloadPin->PinType;

	if (PinType.PinCategory == UEdGraphSchema_K2::PC_Wildcard)
	{
		// PayloadPin에 연결된 것이 없으면 아래 과정 진행 안함
		if (PayloadPin->LinkedTo.Num() == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	UK2Node_TemporaryVariable* TempVarOutput = CompilerContext.SpawnInternalVariable(
		this, PinType.PinCategory, PinType.PinSubCategory, PinType.PinSubCategoryObject.Get(), PinType.ContainerType, PinType.PinValueType);

	// 세팅만 해둠
	UK2Node_CallFunction* const CallGetPayloadNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	CallGetPayloadNode->FunctionReference.SetExternalMember(TEXT("GetPayload"), CurrentProperty->GetOwnerClass());
	CallGetPayloadNode->AllocateDefaultPins();

	UEdGraphPin* GetPayloadCallSelfPin = Schema->FindSelfPin(*CallGetPayloadNode, EGPD_Input);
	if (GetPayloadCallSelfPin)
	{
		bIsErrorFree &= Schema->TryCreateConnection(GetPayloadCallSelfPin, ProxyObjectVar.TempVar->GetVariablePin());

		UEdGraphPin* GetPayloadExecPin = CallGetPayloadNode->FindPinChecked(UEdGraphSchema_K2::PN_Execute);
		UEdGraphPin* GetPayloadThenPin = CallGetPayloadNode->FindPinChecked(UEdGraphSchema_K2::PN_Then);

		UEdGraphPin* LastThenPin = nullptr;
		UEdGraphPin* GetPayloadPin = CallGetPayloadNode->FindPinChecked(TEXT("OutPayload"));
		bIsErrorFree &= Schema->TryCreateConnection(TempVarOutput->GetVariablePin(), GetPayloadPin);

		UK2Node_AssignmentStatement* AssignNode = CompilerContext.SpawnIntermediateNode<UK2Node_AssignmentStatement>(this, SourceGraph);
		AssignNode->AllocateDefaultPins();
		bIsErrorFree &= Schema->TryCreateConnection(GetPayloadThenPin, AssignNode->GetExecPin());
		bIsErrorFree &= Schema->TryCreateConnection(PayloadVar.TempVar->GetVariablePin(), AssignNode->GetVariablePin());
		AssignNode->NotifyPinConnectionListChanged(AssignNode->GetVariablePin());
		bIsErrorFree &= Schema->TryCreateConnection(AssignNode->GetValuePin(), TempVarOutput->GetVariablePin());
		AssignNode->NotifyPinConnectionListChanged(AssignNode->GetValuePin());

		bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*InOutLastActivatedThenPin, *AssignNode->GetThenPin()).CanSafeConnect();
		bIsErrorFree &= Schema->TryCreateConnection(InOutLastActivatedThenPin, GetPayloadExecPin);

		UEdGraphPin* OutActualChannelPin = GetOutputChannelPin();
		bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*OutActualChannelPin, *ActualChannelVar.TempVar->GetVariablePin()).CanSafeConnect();
		
		
	}

	return bIsErrorFree;
	
}

void UK2Node_AsyncAction_ListenForGameplayMessages::RefreshOutputPayloadType()
{
	UEdGraphPin* PayloadPin = GetPayloadPin();
	UEdGraphPin* PayloadTypePin = GetPayloadTypePin();

	// PayloadPin에 아직 한번도 PayloadType의 Object들을 설정하지 않았을 때만 적용.
	if (PayloadTypePin->DefaultObject != PayloadPin->PinType.PinSubCategoryObject)
	{
		if (PayloadPin->SubPins.Num() >0)
		{
			GetSchema()->RecombinePin(PayloadPin);
		}

		PayloadPin->PinType.PinSubCategoryObject = PayloadTypePin->DefaultObject;
		PayloadPin->PinType.PinCategory = (PayloadTypePin->DefaultObject == nullptr) ? UEdGraphSchema_K2::PC_Wildcard : UEdGraphSchema_K2::PC_Struct;
	}
}

UEdGraphPin* UK2Node_AsyncAction_ListenForGameplayMessages::GetPayloadPin() const
{
	UEdGraphPin* Pin = FindPinChecked(UK2Node_AsyncAction_ListenForGameplayMessagesHelper::PayloadPinName);
	check(Pin->Direction == EGPD_Output);
	return Pin;
}

UEdGraphPin* UK2Node_AsyncAction_ListenForGameplayMessages::GetPayloadTypePin() const
{
	
	UEdGraphPin* Pin = FindPinChecked(UK2Node_AsyncAction_ListenForGameplayMessagesHelper::PayloadTypePinName);
	check(Pin->Direction == EGPD_Input);
	return Pin;
}

UEdGraphPin* UK2Node_AsyncAction_ListenForGameplayMessages::GetOutputChannelPin() const
{
	UEdGraphPin* Pin = FindPinChecked(UK2Node_AsyncAction_ListenForGameplayMessagesHelper::ActualChannelPinName);
	check(Pin->Direction == EGPD_Output);
	return Pin;
}
