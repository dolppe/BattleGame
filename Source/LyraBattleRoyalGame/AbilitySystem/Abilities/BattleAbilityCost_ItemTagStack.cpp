#include "BattleAbilityCost_ItemTagStack.h"
#include "NativeGameplayTags.h"
#include "LyraBattleRoyalGame/Equipment/BattleGameplayAbility_FromEquipment.h"
#include "LyraBattleRoyalGame/Inventory/BattleInventoryItemInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleAbilityCost_ItemTagStack)

UE_DEFINE_GAMEPLAY_TAG(TAG_ABILITY_FAIL_COST, "Ability.ActivateFail.Cost")

bool UBattleAbilityCost_ItemTagStack::CheckCost(const UBattleGameplayAbility* Ability,
	const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	FGameplayTagContainer* OptionalRelevantTags) const
{
	if (const UBattleGameplayAbility_FromEquipment* EquipmentAbility = Cast<const UBattleGameplayAbility_FromEquipment>(Ability))
	{
		if (UBattleInventoryItemInstance* ItemInstance = EquipmentAbility->GetAssociatedItem())
		{
			const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

			const float NumStacksReal = Quantity.GetValueAtLevel(AbilityLevel);
			const int32 NumStacks = FMath::TruncToInt(NumStacksReal);
			const bool bCanApplyCost = ItemInstance->GetStatTagStackCount(Tag) >= NumStacks;

			if (!bCanApplyCost && OptionalRelevantTags && FailureTag.IsValid())
			{
				OptionalRelevantTags->AddTag(FailureTag);
			}
			return bCanApplyCost;
		}
	}

	return false;
}

void UBattleAbilityCost_ItemTagStack::ApplyCost(const UBattleGameplayAbility* Ability,
	const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (const UBattleGameplayAbility_FromEquipment* EquipmentAbility = Cast<const UBattleGameplayAbility_FromEquipment>(Ability))
	{
		if (UBattleInventoryItemInstance* ItemInstance = EquipmentAbility->GetAssociatedItem())
		{
			const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

			const float NumStacksReal = Quantity.GetValueAtLevel(AbilityLevel);
			const int32 NumStacks = FMath::TruncToInt(NumStacksReal);

			ItemInstance->RemoveStatTagStack(Tag, NumStacks);
		}
	}

	
}
