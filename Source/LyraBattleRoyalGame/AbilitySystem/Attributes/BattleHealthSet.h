#pragma once

#include "BattleAttributeSet.h"
#include "AbilitySystemComponent.h"
#include "BattleHealthSet.generated.h"

/*
 *	체력에 대한 속성값 관리
 */

UCLASS(BlueprintType)
class UBattleHealthSet : public UBattleAttributeSet
{
	GENERATED_BODY()
public:
	UBattleHealthSet();

	ATTRIBUTE_ACCESSORS(UBattleHealthSet, Health);
	ATTRIBUTE_ACCESSORS(UBattleHealthSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UBattleHealthSet, Healing);
	ATTRIBUTE_ACCESSORS(UBattleHealthSet, Damage);

	mutable FBattleAttributeEvent OnOutOfHealth;
	
protected:
	
	// GameplayEffect가 Attribute를 건드린 후에 불리는 콜백
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;
	
	bool bOutOfHealth;

private:
	
	UPROPERTY(BlueprintReadOnly, Category="Battle|Health", meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, Category="Battle|Health", meta=(AllowPrivateAccess=true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category="Battle|Health", meta=(AllowPrivateAccess=true))
	FGameplayAttributeData Healing;

	UPROPERTY(BlueprintReadOnly, Category="Battle|Health", meta=(HideFromModifiers, AllowPrivateAccess=true))
	FGameplayAttributeData Damage;
	
};

