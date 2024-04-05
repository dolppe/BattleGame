#pragma once

#include "GameplayTagContainer.h"
#include "BattleGameplayTagStack.generated.h"

USTRUCT(BlueprintType)
struct FBattleGameplayTagStack
{
	GENERATED_BODY()

	FBattleGameplayTagStack() {}
	FBattleGameplayTagStack(FGameplayTag InTag, int32 InStackCount)
		: Tag(InTag)
		, StackCount(InStackCount)
	{}

	UPROPERTY()
	FGameplayTag Tag;

	UPROPERTY()
	int32 StackCount = 0;
	
};

USTRUCT(BlueprintType)
struct FBattleGameplayTagStackContainer
{
	GENERATED_BODY()

	FBattleGameplayTagStackContainer() {}

	void AddStack(FGameplayTag Tag, int32 StackCount);
	void RemoveStack(FGameplayTag Tag, int32 StackCount);
	
	int32 GetStackCount(FGameplayTag Tag) const
	{
		return TagToCountMap.FindRef(Tag);
	}

	bool ContainsTag(FGameplayTag Tag) const
	{
		return TagToCountMap.Contains(Tag);
	}

	UPROPERTY()
	TArray<FBattleGameplayTagStack> Stacks;

	// GameplayTag를 통해 해당 Tag의 StackCount에 빠르게 접근 가능.
	TMap<FGameplayTag, int32> TagToCountMap;
	
};

