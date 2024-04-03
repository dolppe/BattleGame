#pragma once

#include "CommonUserWidget.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "BattleWeaponUserInterface.generated.h"

class UBattleWeaponInstance;

UCLASS()
class UBattleWeaponUserInterface : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	UBattleWeaponUserInterface(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	// Weapon 변경에 따른 BP Event
	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponChanged(UBattleWeaponInstance* OldWeapon, UBattleWeaponInstance* NewWeapon);

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	// 현재 장착된 WeaponInstance를 추적함 (NativeTick을 통해 주기적으로 업데이트)
	UPROPERTY(Transient)
	TObjectPtr<UBattleWeaponInstance> CurrentInstance;
	
};
