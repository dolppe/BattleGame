#pragma once

#include "AttributeSet.h"
#include "Delegates/Delegate.h"
#include "BattleAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

/*
 *	메인 Attribute Set Class임.	
 */

struct FGameplayEffectSpec;
class AActor;

DECLARE_MULTICAST_DELEGATE_FourParams(FBattleAttributeEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayEffectSpec&, float /*Magnitude*/);

UCLASS()
class UBattleAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
public:
	UBattleAttributeSet();
	
};


