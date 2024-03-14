#pragma once

#include "Components/PawnComponent.h"
#include "BattleEquipmentManagerComponent.generated.h"

class UBattleEquipmentDefinition;
class UBattleEquipmentInstance;


USTRUCT(BlueprintType)
struct FBattleAppliedEquipmentEntry
{
	GENERATED_BODY()

	// 장착물에 대한 메타 데이터(정의)
	UPROPERTY()
	TSubclassOf<UBattleEquipmentDefinition> EquipmentDefinition;

	// EquipmentDefinition을 통해 생성된 인스턴스
	UPROPERTY()
	TObjectPtr<UBattleEquipmentInstance> Instance = nullptr;
	
};

USTRUCT(BlueprintType)
struct FBattleEquipmentList
{
	GENERATED_BODY()

	FBattleEquipmentList(UActorComponent* InOwnerComponent = nullptr)
		: OwnerComponent(InOwnerComponent)
	{}
	
	UPROPERTY()
	TArray<FBattleAppliedEquipmentEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};



// Pawn의 COmponent로서 장착물에 대한 관리를 담당한다.
UCLASS(BlueprintType)
class UBattleEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()
public:
	UBattleEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	UPROPERTY()
	FBattleEquipmentList EquipmentList;
};

