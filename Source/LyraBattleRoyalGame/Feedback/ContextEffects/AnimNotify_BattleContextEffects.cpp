#include "AnimNotify_BattleContextEffects.h"

#include "BattleContextEffectsInterface.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(AnimNotify_BattleContextEffects)

UAnimNotify_BattleContextEffects::UAnimNotify_BattleContextEffects()
{
}

void UAnimNotify_BattleContextEffects::PostLoad()
{
	Super::PostLoad();
}

FString UAnimNotify_BattleContextEffects::GetNotifyName_Implementation() const
{
	if (Effect.IsValid())
	{
		return Effect.ToString();
	}
	
	return Super::GetNotifyName_Implementation();
}

PRAGMA_DISABLE_OPTIMIZATION

void UAnimNotify_BattleContextEffects::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		if (AActor* OwningActor = MeshComp->GetOwner())
		{
			bool bHitSuccess = false;
			FHitResult HitResult;
			FCollisionQueryParams QueryParams;

			if (TraceProperties.bIgnoreActor)
			{
				QueryParams.AddIgnoredActor(OwningActor);
			}

			QueryParams.bReturnPhysicalMaterial = true;

			if (bPerformTrace)
			{
				FVector TraceStart = bAttached ? MeshComp->GetSocketLocation(SocketName) : MeshComp->GetComponentLocation();

				if (UWorld* World = OwningActor->GetWorld())
				{
					bHitSuccess = World->LineTraceSingleByChannel(HitResult, TraceStart, (TraceStart + TraceProperties.EndTraceLocationOffset),
						TraceProperties.TraceChannel, QueryParams, FCollisionResponseParams::DefaultResponseParam);
				}
			}

			FGameplayTagContainer Contexts;

			TArray<UObject*> BattleContextEffectImplementingObjects;

			if (OwningActor->Implements<UBattleContextEffectsInterface>())
			{
				BattleContextEffectImplementingObjects.Add(OwningActor);
			}

			for (const auto Component : OwningActor->GetComponents())
			{
				if (Component)
				{
					if (Component->Implements<UBattleContextEffectsInterface>())
					{
						BattleContextEffectImplementingObjects.Add(Component);
					}
				}
			}

			for (UObject* BattleContextEffectImplementingObject : BattleContextEffectImplementingObjects)
			{
				if (BattleContextEffectImplementingObject)
				{
					IBattleContextEffectsInterface::Execute_AnimMotionEffect(BattleContextEffectImplementingObject,
						(bAttached ? SocketName : FName("None")),
						Effect, MeshComp, LocationOffset, RotationOffset,
						Animation, bHitSuccess, HitResult, Contexts, VFXProperties.Scale,
						AudioProperties.VolumeMultiplier, AudioProperties.PitchMultiplier);
				}
			}
		}
	}
}

PRAGMA_ENABLE_OPTIMIZATION





