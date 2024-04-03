#pragma once

#include "AttributeSet.h"
#include "BattleAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/*
 *	메인 Attribute Set Class임.	
 */

UCLASS()
class UBattleAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UBattleAttributeSet();
	
};


