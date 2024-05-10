#pragma once

#include "Components/ControllerComponent.h"
#include "BattleWeaponStateComponent.generated.h"


UCLASS()
class UBattleWeaponStateComponent : public UControllerComponent
{
	GENERATED_BODY()
public:

	UBattleWeaponStateComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	
};
