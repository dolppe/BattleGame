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
	ATTRIBUTE_ACCESSORS(UBattleCombatSet, BaseDamage);


private:

	// Healing의 단위
	UPROPERTY(BlueprintReadOnly, Category="Battle|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseHeal;

	UPROPERTY(BlueprintReadOnly, Category="Battle|Combat", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData BaseDamage;
	
};
