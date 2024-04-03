#pragma once

#include "AbilitySystemComponent.h"
#include "BattleAttributeSet.h"
#include "BattleCombatSet.generated.h"

/*
 *	전투와 관련된 Attribute를 담고 있는 Set
 *	추후 BaseDamage 등을 추가 해야함
 */

UCLASS(BlueprintType)
class UBattleCombatSet : public UBattleAttributeSet
{
	GENERATED_BODY()
public:
	UBattleCombatSet();

	ATTRIBUTE_ACCESSORS(UBattleCombatSet, BaseHeal);

	// Healing의 단위
	UPROPERTY(BlueprintReadOnly, Category="Battle|Combat")
	FGameplayAttributeData BaseHeal;
	
};
