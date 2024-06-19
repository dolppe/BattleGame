#pragma once

#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "BattleContextEffectsInterface.generated.h"

UINTERFACE()
class LYRABATTLEROYALGAME_API UBattleContextEffectsInterface : public UInterface
{
	GENERATED_BODY()
};

class LYRABATTLEROYALGAME_API IBattleContextEffectsInterface : public IInterface
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AnimMotionEffect(const FName Bone
		, const FGameplayTag MotionEffect
		, USceneComponent* StaticMeshComponent
		, const FVector LocationOffset
		, const FRotator RotationOffset
		, const UAnimSequenceBase* AnimationSequence
		, const bool bHitSuccess
		, const FHitResult HitResult
		, FGameplayTagContainer Contexts
		, FVector VFXScale = FVector(1)
		, float AudioVolume = 1
		, float AudioPitch = 1);
	
};
