#pragma once

#include "AbilitySystemComponent.h"
#include "BattleAbilitySystemComponent.generated.h"

UCLASS()
class UBattleAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:

	UBattleAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};

