#include "BattleHealthSet.h"

#include "GameplayEffectExtension.h"
#include "LyraBattleRoyalGame/AbilitySystem/BattleAbilitySystemComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleHealthSet)


UE_DEFINE_GAMEPLAY_TAG(TAG_Gameplay_DamageImmunity, "Gameplay.DamageImmunity");

UBattleHealthSet::UBattleHealthSet()
	: Super()
	, Health(50.0f)
	, MaxHealth(100.0f)
{
	bOutOfHealth = false;
}

bool UBattleHealthSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	if (!Super::PreGameplayEffectExecute(Data))
	{
		return false;
	}

	if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		if (Data.EvaluatedData.Magnitude > 0.0f)
		{
			if(Data.Target.HasMatchingGameplayTag(TAG_Gameplay_DamageImmunity))
			{
				Data.EvaluatedData.Magnitude = 0.0f;
				return false;
			}
		}
	}

	return true;
}

void UBattleHealthSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float MinimumHealth = 0.0f;

	// Healing을 Health에 적용
	if (Data.EvaluatedData.Attribute == GetHealingAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth() + GetHealing(), MinimumHealth, GetMaxHealth()));
		SetHealing(0.0f);
	}
	else if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), MinimumHealth, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetDamageAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth() - GetDamage(), MinimumHealth, GetMaxHealth()));
		SetDamage(0.0f);
	}

	if ((GetHealth() <= 0.0f) && !bOutOfHealth)
	{
		if (OnOutOfHealth.IsBound())
		{
			const FGameplayEffectContextHandle& EffectContext = Data.EffectSpec.GetEffectContext();
			AActor* Instigator = EffectContext.GetOriginalInstigator();
			AActor* Causer = EffectContext.GetEffectCauser();

			OnOutOfHealth.Broadcast(Instigator, Causer, Data.EffectSpec, Data.EvaluatedData.Magnitude);
		}

		bOutOfHealth = (GetHealth() <= 0.0f);
	}
}

void UBattleHealthSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	// HealthAttribute는 0~MaxHealth값을 가지도록
	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	// MaxHealthAttribute는 1미만이 되지 않도록 설정
	else if (Attribute == GetMaxHealthAttribute())
	{
		NewValue = FMath::Max( NewValue, 1.0f);
	}
}

void UBattleHealthSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UBattleHealthSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UBattleHealthSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);


	if (bOutOfHealth && (GetHealth() >0.0f))
	{
		bOutOfHealth = false;
	}
}
