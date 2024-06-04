#pragma once

#include "BattleGameplayAbility.h"
#include "GameplayAbilitySpec.h"
#include "BattleGameplayAbility_Reset.generated.h"

UCLASS()
class LYRABATTLEROYALGAME_API UBattleGameplayAbility_Reset : public UBattleGameplayAbility
{
	GENERATED_BODY()
public:

	UBattleGameplayAbility_Reset(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:

	void DoneAddingNativeTags();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	
};


USTRUCT(BlueprintType)
struct FBattlePlayerResetMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AActor> OwnerPlayerState = nullptr;
	
};
