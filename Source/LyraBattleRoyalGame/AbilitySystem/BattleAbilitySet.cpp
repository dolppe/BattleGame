#include "BattleAbilitySet.h"
#include "Abilities/BattleGameplayAbility.h"
#include "BattleAbilitySystemComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleAbilitySet)

void FBattleAbilitySet_GrantedHandles::AddAbilitySpecHandle(const FGameplayAbilitySpecHandle& Handle)
{
	if (Handle.IsValid())
	{
		AbilitySpecHandles.Add(Handle);
	}
}

void FBattleAbilitySet_GrantedHandles::TakeFromAbilitySystem(UBattleAbilitySystemComponent* BattleASC)
{
	if (!BattleASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : AbilitySpecHandles)
	{
		if (Handle.IsValid())
		{
			BattleASC->ClearAbility(Handle);
		}
		
	}
	
}

UBattleAbilitySet::UBattleAbilitySet(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UBattleAbilitySet::GiveToAbilitySystem(UBattleAbilitySystemComponent* BattleASC,
	FBattleAbilitySet_GrantedHandles* OutGrantedHandles, UObject* SourceObject)
{
	check(BattleASC);

	if (!BattleASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (int32 AbilityIndex = 0; AbilityIndex < GrantedGameplayAbilities.Num(); ++ AbilityIndex)
	{
		const FBattleAbilitySet_GameplayAbility& AbilityToGrant = GrantedGameplayAbilities[AbilityIndex];
		if (!IsValid(AbilityToGrant.Ability))
		{
			continue;
		}

		UBattleGameplayAbility* AbilityCDO = AbilityToGrant.Ability->GetDefaultObject<UBattleGameplayAbility>();

		FGameplayAbilitySpec AbilitySpec(AbilityCDO, AbilityToGrant.AbilityLevel);
		AbilitySpec.SourceObject = SourceObject;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilityToGrant.InputTag);

		const FGameplayAbilitySpecHandle AbilitySpecHandle = BattleASC->GiveAbility(AbilitySpec);
		if (OutGrantedHandles)
		{
			OutGrantedHandles->AddAbilitySpecHandle(AbilitySpecHandle);
		}
		
		
	}
	
}
