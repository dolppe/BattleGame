#pragma once

#include "Components/ActorComponent.h"
#include "BattleInventoryManagerComponent.generated.h"

class UBattleInventoryItemDefinition;
class UBattleInventoryItemInstance;

USTRUCT()
struct FBattleInventoryEntry
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UBattleInventoryItemInstance> Instance= nullptr;
};

// Inventory Item 관리 객체
USTRUCT()
struct FBattleInventoryList
{
	GENERATED_BODY()

	FBattleInventoryList(UActorComponent* InOwnerComponent = nullptr)
		:OwnerComponent(InOwnerComponent)
	{}

	UBattleInventoryItemInstance* AddEntry(TSubclassOf<UBattleInventoryItemDefinition> ItemDef);
	
	
	UPROPERTY()
	TArray<FBattleInventoryEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};


// PlayerCOntroller의 Component로서 Inventroy를 관리한다.
// UActorComponent가 아니라 UControllerComponent를 상속 받아도 무방할 것 같긴 함.
UCLASS(BlueprintType)
class UBattleInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UBattleInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// InventoryItemDefinition을 통해 InventoryList에 추가하여 관리하며, InventoryItemInstance를 반환함.
	UFUNCTION(BlueprintCallable, Category=Inventory)
	UBattleInventoryItemInstance* AddItemDefinition(TSubclassOf<UBattleInventoryItemDefinition> ItemDef);

	UFUNCTION(BlueprintCallable, Category=Inventory, BlueprintPure)
	UBattleInventoryItemInstance* FindFirstItemStackByDefinition(TSubclassOf<UBattleInventoryItemDefinition> ItemDef) const;
	
	UPROPERTY()
	FBattleInventoryList InventoryList;
	
};
