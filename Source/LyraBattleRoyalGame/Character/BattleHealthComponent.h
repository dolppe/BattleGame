#pragma once

#include "Components/GameFrameworkComponent.h"
#include "Delegates/Delegate.h"
#include "LyraBattleRoyalGame/AbilitySystem/BattleGameplayAbilityTargetData_SingleTargetHit.h"
#include "BattleHealthComponent.generated.h"

/*
 *	Character에 대해 체력 관리 처리를 담당하는 Component
 *	 - Blueprintable로 처리해서 Delegate를 UI에 바인딩할 수 있게함
 */

class UBattleAbilitySystemComponent;
class UBattleHealthSet;
class AActor;
class UBattleHealthComponent;
struct FOnAttributeChangeData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FBattleHealth_AttributeChanged, UBattleHealthComponent*, HealthComponent, float, OldValue, float, NewValue, AActor*, Instigator);

UCLASS(Blueprintable)
class UBattleHealthComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()
public:
	UBattleHealthComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintPure, Category="Battle|Health")
	static UBattleHealthComponent* FindHealthComponent(const AActor* Actor);

	UFUNCTION(BlueprintCallable, Category="Battle|Health")
	float GetHealth() const;

	UFUNCTION(BlueprintCallable, Category="Battle|Health")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category="Battle|Health")
	float GetHealthNormalized() const;

	void InitializeWithAbilitySystem(UBattleAbilitySystemComponent* InASC);
	void UnInitializeWithAbilitySystem();

	void HandleHealthChanged(const FOnAttributeChangeData& ChangeData);

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	// HealthSet에 접근하기 위한 ASC를 캐싱해둠
	UPROPERTY()
	TObjectPtr<UBattleAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<const UBattleHealthSet> HealthSet;
	
	UPROPERTY(BlueprintAssignable)
	FBattleHealth_AttributeChanged OnHealthChanged;
};

