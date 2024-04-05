#include "BattleGameplayTagStack.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleGameplayTagStack)

void FBattleGameplayTagStackContainer::AddStack(FGameplayTag Tag, int32 StackCount)
{
	if (!Tag.IsValid())
	{
		return;
	}

	if (StackCount >0)
	{
		for (FBattleGameplayTagStack& Stack : Stacks)
		{
			if (Stack.Tag == Tag)
			{
				const int32 NewCount = Stack.StackCount + StackCount;
				Stack.StackCount = NewCount;
				TagToCountMap[Tag] = NewCount;
				return;
			}
		}

		FBattleGameplayTagStack& NewStack = Stacks.Emplace_GetRef(Tag, StackCount);
		
		TagToCountMap.Add(Tag, StackCount);
	}
	
}

void FBattleGameplayTagStackContainer::RemoveStack(FGameplayTag Tag, int32 StackCount)
{
	if (!Tag.IsValid())
	{
		return;
	}

	if (StackCount > 0)
	{
		for (auto It = Stacks.CreateIterator(); It;++It)
		{
			FBattleGameplayTagStack& Stack = *It;
			if (Stack.Tag == Tag)
			{
				if (Stack.StackCount <= StackCount)
				{
					It.RemoveCurrent();
					TagToCountMap.Remove(Tag);
				}
				else
				{
					const int32 NewCount = Stack.StackCount - StackCount;
					Stack.StackCount = NewCount;
					TagToCountMap[Tag] = NewCount;
				}
				return;
			}
		}
		
	}
	
}
