#pragma once

#include "AbilitySystemComponent.h"
#include "BattleAbilitySystemComponent.generated.h"

UCLASS()
class UBattleAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:

	UBattleAbilitySystemComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ProcessAbilityInput(float DeltaTime, bool bGamePaused);
	
	// 어빌리티 인풋을 처리할 팬딩 큐
	TArray<FGameplayAbilitySpecHandle> InputPressedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputReleasedSpecHandles;
	TArray<FGameplayAbilitySpecHandle> InputHeldSpecHandles;
	
};

