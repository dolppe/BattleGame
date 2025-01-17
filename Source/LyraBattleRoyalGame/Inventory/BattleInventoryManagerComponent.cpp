#include "BattleInventoryManagerComponent.h"

#include "BattleInventoryItemDefinition.h"
#include "BattleInventoryItemInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleInventoryManagerComponent)

TArray<UBattleInventoryItemInstance*> FBattleInventoryList::GetAllItems() const
{
	TArray<UBattleInventoryItemInstance*> Results;
	Results.Reserve(Entries.Num());
	for (const FBattleInventoryEntry& Entry : Entries)
	{
		if (Entry.Instance != nullptr)
		{
			Results.Add(Entry.Instance);
		}
	}

	return Results;
}

UBattleInventoryItemInstance* FBattleInventoryList::AddEntry(TSubclassOf<UBattleInventoryItemDefinition> ItemDef)
{
	UBattleInventoryItemInstance* Result = nullptr;
	check(ItemDef);
	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();
	check(OwningActor->HasAuthority());

	FBattleInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UBattleInventoryItemInstance>(OwningActor);
	NewEntry.Instance->ItemDef = ItemDef;

	for (UBattleInventoryItemFragment* Fragment : GetDefault<UBattleInventoryItemDefinition>(ItemDef)->Fragments)
	{
		if (Fragment)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}
	
	Result = NewEntry.Instance;
	return Result;	
	
}

void FBattleInventoryList::RemoveEntry(UBattleInventoryItemInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator();EntryIt;++EntryIt)
	{
		FBattleInventoryEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			EntryIt.RemoveCurrent();
		}
	}
}

UBattleInventoryManagerComponent::UBattleInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
{
}

UBattleInventoryItemInstance* UBattleInventoryManagerComponent::AddItemDefinition(
	TSubclassOf<UBattleInventoryItemDefinition> ItemDef)
{
	UBattleInventoryItemInstance* Result = nullptr;
	
	if (ItemDef)
	{
		Result = InventoryList.AddEntry(ItemDef);
	}

	return Result;
	
}

UBattleInventoryItemInstance* UBattleInventoryManagerComponent::FindFirstItemStackByDefinition(
	TSubclassOf<UBattleInventoryItemDefinition> ItemDef) const
{
	for (const FBattleInventoryEntry& Entry : InventoryList.Entries)
	{
		UBattleInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				return Instance;
			}
		}
	}
	
	return nullptr;
	
}

void UBattleInventoryManagerComponent::RemoveItemInstance(UBattleInventoryItemInstance* ItemInstance)
{
	InventoryList.RemoveEntry(ItemInstance);

	if (ItemInstance && IsUsingRegisteredSubObjectList())
	{
		RemoveReplicatedSubObject(ItemInstance);
	}
}

TArray<UBattleInventoryItemInstance*> UBattleInventoryManagerComponent::GetAllItems() const
{
	return InventoryList.GetAllItems();
}
