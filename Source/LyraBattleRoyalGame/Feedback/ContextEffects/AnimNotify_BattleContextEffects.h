#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameplayTagContainer.h"
#include "AnimNotify_BattleContextEffects.generated.h"


USTRUCT(BlueprintType)
struct LYRABATTLEROYALGAME_API FBattleContextEffectAnimNotifyVFXSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=FX)
	FVector Scale = FVector(1.0f,1.0f,1.0f);
	
};


USTRUCT(BlueprintType)
struct LYRABATTLEROYALGAME_API FBattleContextEffectAnimNotifyAudioSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Sound)
	float VolumeMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Sound)
	float PitchMultiplier = 1.0f;
};

USTRUCT(BlueprintType)
struct LYRABATTLEROYALGAME_API FBattleContextEffectAnimNotifyTraceSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Trace)
	TEnumAsByte<ECollisionChannel> TraceChannel = ECollisionChannel::ECC_Visibility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Trace)
	FVector EndTraceLocationOffset = FVector(0.0f,0.0f,-50.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= Trace)
	bool bIgnoreActor = true;
	
};

UCLASS(Const, hidecategories=Object, CollapseCategories, Config = Game, meta=(DisplayName="Play Context Effects"))
class LYRABATTLEROYALGAME_API UAnimNotify_BattleContextEffects : public UAnimNotify
{
	GENERATED_BODY()
public:

	UAnimNotify_BattleContextEffects();

	virtual void PostLoad() override;

	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (DisplayName = "Effect", ExposeOnSpawn = true))
	FGameplayTag Effect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
	FVector LocationOffset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
	FRotator RotationOffset = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
	FBattleContextEffectAnimNotifyVFXSettings VFXProperties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
	FBattleContextEffectAnimNotifyAudioSettings AudioProperties;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttachmentProperties", meta = (ExposeOnSpawn = true))
	uint32 bAttached : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttachmentProperties", meta = (ExposeOnSpawn = true, EditCondition="bAttached"))
	FName SocketName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
	uint32 bPerformTrace : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimNotify", meta = (ExposeOnSpawn = true))
	FBattleContextEffectAnimNotifyTraceSettings TraceProperties;
	
	
};

