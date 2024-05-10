#pragma once

#include "CommonUserWidget.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"
#include "BattleReticleWidgetBase.generated.h"

class UBattleWeaponInstance;
class UBattleInventoryItemInstance;

UCLASS()
class UBattleReticleWidgetBase : public UCommonUserWidget
{
	GENERATED_BODY()
public:
	UBattleReticleWidgetBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	void InitializeFromWeapon(UBattleWeaponInstance* InWeapon);

	UFUNCTION(BlueprintImplementableEvent)
	void OnWeaponInitialized();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float ComputeSpreadAngle() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float ComputeMaxScreenspaceSpreadRadius() const;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool HasFirstShotAccuracy() const;

protected:
	/*
	 * 각 변수들은 상태 추적용으로 캐싱해두는 것임.
	 */
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UBattleWeaponInstance> WeaponInstance;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UBattleInventoryItemInstance> InventoryInstance;
	
};

