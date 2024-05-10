#include "BattleWeaponInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleWeaponInstance)

UBattleWeaponInstance::UBattleWeaponInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UBattleWeaponInstance::UpdateFiringTime()
{
	UWorld* World = GetWorld();
	check(World);
	TimeLastFired = World->GetTimeSeconds();
}

TSubclassOf<UAnimInstance> UBattleWeaponInstance::PickBestAnimLayer(bool bEquipped,
                                                                    const FGameplayTagContainer& CosmeticTags) const
{
	const FBattleAnimLayerSelectionSet& SetToQuery = (bEquipped ? EquippedAnimSet : UnequippedAnimSet);
	return SetToQuery.SelectBestLayer(CosmeticTags);
}
