#pragma once

#include "CoreMinimal.h"
#include "LyraBattleRoyalGame/Equipment/BattleGameplayAbility_FromEquipment.h"
#include "BattleGameplayAbility_RangedWeapon.generated.h"

class UBattleRangedWeaponInstance;

// 어디를 대상으로 Target으로 설정할 건지
UENUM()
enum class EBattleAbilityTargetingSource : uint8
{
	// 카메라 기준으로 진행
	CameraTowardsFocus,
};

UCLASS()
class UBattleGameplayAbility_RangedWeapon : public UBattleGameplayAbility_FromEquipment
{
	GENERATED_BODY()
public:

	struct FRangedWeaponFiringInput
	{
		FVector StartTrace;
		FVector EndAim;
		FVector AimDir;
		UBattleRangedWeaponInstance* WeaponData = nullptr;
		bool bCanPlayBulletFX = false;

		FRangedWeaponFiringInput()
			: StartTrace(ForceInitToZero), EndAim(ForceInitToZero), AimDir(ForceInitToZero) 
		{}
	};
	
	UBattleGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());


	ECollisionChannel DetermineTraceChannel(FCollisionQueryParams& TraceParams, bool bIsSimulated) const;
	void AddAdditionalTraceIgnoreActors(FCollisionQueryParams& TraceParams) const;
	FHitResult WeaponTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHitResults) const;
	int32 FindFirstPawnHitResult(const TArray<FHitResult>& HitResults) const;
	FHitResult DoSingleBulletTrace(const FVector& StartTrace, const FVector& EndTrace, float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHits) const;
	void TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData, TArray<FHitResult>& OutHits);

	UBattleRangedWeaponInstance* GetWeaponInstance();
	FVector GetWeaponTargetingSourceLocation() const;
	FTransform GetTargetingTransform(APawn* SourcePawn, EBattleAbilityTargetingSource Source);
	void PerformLocalTargeting(TArray<FHitResult>& OutHits);
	void OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData, FGameplayTag ApplicationTag);
	
	
	UFUNCTION(BlueprintCallable)
	void StartRangedWeaponTargeting();

	UFUNCTION(BlueprintImplementableEvent)
	void OnRangeWeaponTargetDataReady(const FGameplayAbilityTargetDataHandle& TargetData);
	
};
