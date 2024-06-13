#pragma once

#include "AbilitySystemComponent.h"
#include "Abilities/BattleGameplayAbility.h"
#include "GameplayAbilitySpec.h"
#include "GameplayTagContainer.h"
#include "Templates/Function.h"
#include "BattleAbilitySystemComponent.generated.h"

class UBattleAbilityTagRelationshipMapping;

UCLASS()
class UBattleAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:

	UBattleAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	typedef TFunctionRef<bool(const UBattleGameplayAbility* LyraAbility, FGameplayAbilitySpecHandle Handle)> TShouldCancelAbilityFunc;
	void CancelAbilityByFunc(TShouldCancelAbilityFunc ShouldCancelFunc, bool bReplicateCancelAbility);
	
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor) override;
	
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	void ClearAbilityInput();


	void SetTagRelationshipMapping(UBattleAbilityTagRelationshipMapping* NewMapping);
	void GetAdditionalActivationTagRequirements(const FGameplayTagContainer& AbilityTags, FGameplayTagContainer& OutActivationRequired, FGameplayTagContainer& OutActivationBlocked) const;

protected:

	void TryActivateAbilitiesOnSpawn();
	
	virtual void AbilitySpecInputPressed(FGameplayAbilitySpec& Spec) override;
	virtual void AbilitySpecInputReleased(FGameplayAbilitySpec& Spec) override;
	virtual void ApplyAbilityBlockAndCancelTags(const FGameplayTagContainer& AbilityTags, UGameplayAbility* RequestingAbility, bool bEnableBlockTags, const FGameplayTagContainer& BlockTags, bool bExecuteCancelTags, const FGameplayTagContainer& CancelTags) override;
	
	
protected:

	UPROPERTY()
	TObjectPtr<UBattleAbilityTagRelationshipMapping> TagRelationshipMapping;
	
	// 어빌리티 인풋을 처리할 팬딩 큐
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
	
};

