#include "BattleGameplayAbility_RangedWeapon.h"

#include "AbilitySystemComponent.h"
#include "BattleRangedWeaponInstance.h"
#include "LyraBattleRoyalGame/BattleLogChannels.h"
#include "LyraBattleRoyalGame/AbilitySystem/BattleGameplayAbilityTargetData_SingleTargetHit.h"
#include "LyraBattleRoyalGame/Physics/BattleCollisionChannels.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleGameplayAbility_RangedWeapon)

UBattleGameplayAbility_RangedWeapon::UBattleGameplayAbility_RangedWeapon(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UBattleGameplayAbility_RangedWeapon::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	UBattleRangedWeaponInstance* WeaponData = GetWeaponInstance();
	check(WeaponData);
	WeaponData->UpdateFiringTime();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

ECollisionChannel UBattleGameplayAbility_RangedWeapon::DetermineTraceChannel(FCollisionQueryParams& TraceParams,
                                                                             bool bIsSimulated) const
{
	return Battle_TraceChannel_Weapon;
}

void UBattleGameplayAbility_RangedWeapon::AddAdditionalTraceIgnoreActors(FCollisionQueryParams& TraceParams) const
{
	if (AActor* Avatar = GetAvatarActorFromActorInfo())
	{
		TArray<AActor*> AttachedActors;

		// 해당 함수는 계층적으로 모든 Actor를 추출함
		// 근데 FindFirstPawnHitResult는 한단계만 추출함.
		Avatar->GetAttachedActors(AttachedActors);
		
		TraceParams.AddIgnoredActors(AttachedActors);		
	}
}

FHitResult UBattleGameplayAbility_RangedWeapon::WeaponTrace(const FVector& StartTrace, const FVector& EndTrace,
                                                            float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHitResults) const
{
	TArray<FHitResult> HitResults;

	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(WeaponTrace), true, GetAvatarActorFromActorInfo());
	TraceParams.bReturnPhysicalMaterial = true;

	AddAdditionalTraceIgnoreActors(TraceParams);

	const ECollisionChannel TraceChannel = DetermineTraceChannel(TraceParams, bIsSimulated);

	if (SweepRadius > 0.0f)
	{
		GetWorld()->SweepMultiByChannel(HitResults, StartTrace, EndTrace, FQuat::Identity,TraceChannel, FCollisionShape::MakeSphere(SweepRadius), TraceParams);
	}
	else
	{
		GetWorld()->LineTraceMultiByChannel(HitResults, StartTrace, EndTrace, TraceChannel, TraceParams);
	}

	FHitResult Hit(ForceInit);
	if (HitResults.Num() >0)
	{
		for (FHitResult& CurHitResult : HitResults)
		{
			auto Pred = [&CurHitResult] (const FHitResult& Other)
			{
				return Other.HitObjectHandle == CurHitResult.HitObjectHandle;
			};

			if (!OutHitResults.ContainsByPredicate(Pred))
			{
				OutHitResults.Add(CurHitResult);
				UE_LOG(LogBattle, Log, TEXT("PhysicalMat: %s"),*(CurHitResult.PhysMaterial->GetName()));
			}
		}
		Hit = OutHitResults.Last();
	}
	else
	{
		Hit.TraceStart = StartTrace;
		Hit.TraceEnd = EndTrace;
	}

	return Hit;
	
}

int32 UBattleGameplayAbility_RangedWeapon::FindFirstPawnHitResult(const TArray<FHitResult>& HitResults) const
{
	for (int32 Idx = 0; Idx < HitResults.Num() ; ++Idx)
	{
		const FHitResult& CurHitResult = HitResults[Idx];
		
		if (CurHitResult.HitObjectHandle.DoesRepresentClass(APawn::StaticClass()))
		{
			return Idx;
		}
		else
		{
			AActor* HitActor = CurHitResult.HitObjectHandle.FetchActor();

			if ((HitActor != nullptr) && (HitActor->GetAttachParentActor() != nullptr) && (Cast<APawn>(HitActor->GetAttachParentActor()) != nullptr))
			{
				return Idx;
			}
		}
	}

	return INDEX_NONE;
}

FHitResult UBattleGameplayAbility_RangedWeapon::DoSingleBulletTrace(const FVector& StartTrace, const FVector& EndTrace,
                                                                    float SweepRadius, bool bIsSimulated, TArray<FHitResult>& OutHits) const
{
	FHitResult Impact;

	if (FindFirstPawnHitResult(OutHits) == INDEX_NONE)
	{
		Impact = WeaponTrace(StartTrace, EndTrace, 0.0f, bIsSimulated, OutHits);
	}

	if (FindFirstPawnHitResult(OutHits) == INDEX_NONE)
	{
		if (SweepRadius > 0.0f)
		{
			TArray<FHitResult> SweepHits;
			Impact = WeaponTrace(StartTrace, EndTrace, SweepRadius, bIsSimulated, SweepHits);

			const int32 FirstPawnIdx = FindFirstPawnHitResult(SweepHits);
			if (SweepHits.IsValidIndex(FirstPawnIdx))
			{
				bool bUseSweepHits = true;
				for (int32 Idx = 0; Idx < FirstPawnIdx; ++ Idx)
				{
					const FHitResult& CurHitResult = SweepHits[Idx];
					auto Pred = [&CurHitResult](const FHitResult& Other)
					{
						return Other.HitObjectHandle == CurHitResult.HitObjectHandle;
					};

					if (CurHitResult.bBlockingHit && OutHits.ContainsByPredicate(Pred))
					{
						bUseSweepHits = false;
						break;
					}
				}

				if (bUseSweepHits)
				{
					OutHits = SweepHits;
				}
			}
			
		}
	}

	return Impact;
}

FVector VRandConeNormalDistribution(const FVector& Dir, const float ConeHalfAngleRad, const float Exponent)
{
	// 탄퍼짐 정도가 존재
	if (ConeHalfAngleRad >0.f)
	{
		// 탄퍼짐 정도에 따라 정해진 원뿔의 최대 각도
		// 원뿔이기에 절반만 사용
		const float ConeHalfAngleDegrees = FMath::RadiansToDegrees(ConeHalfAngleRad);

		// 탄퍼짐은 원뿔을 활용하여 계산하고, 이는 두 가지 Rotation이 필요함
		// 첫 번째로 원의 지름 라인에 대해 퍼지는 각도 => YAW에 적용될 각도이며, 이는 탄퍼짐 최대각을 넘지 않도록 해야함
		// 최초의 원의 지름이 가로로 일자라고 했을 때, 이 Rotation만 이용한다면, 탄이 왼쪽이나 오른쪽으로만 튀게 됨. 
		// 두 번째로 원에서 원의 지름을 돌리는 Rotation => Roll에 적용될 각도이며, 이는 0~360도가 될 수 있음
		// 만약 이 Rotation만 이용한다면, 탄은 가운데로만 박힐 것임.
		// 이 둘을 합쳐서 AngleAround로 원의 지름 중 어느 각도의 원의 지름을 사용할지를 정하고
		// AngleFromCenter로 원의 중심으로부터 어느 정도의 각도로 탄이 퍼질지를 정하여 이를 합침
		// 그림으로 이해하면 편함
		const float FromCenter = FMath::Pow(FMath::FRand(), Exponent);
		const float AngleFromCenter = FromCenter * ConeHalfAngleDegrees;
		const float AngleAround = FMath::FRand() * 360.0f;

		FRotator Rot = Dir.Rotation();
		FQuat DirQuat(Rot);
		FQuat FromCenterQuat(FRotator(0.0f, AngleFromCenter, 0.0f));
		FQuat AroundQuat(FRotator(0.0f, 0.0, AngleAround));
		FQuat FinalDirectionQuat = DirQuat * AroundQuat * FromCenterQuat;
		FinalDirectionQuat.Normalize();

		return FinalDirectionQuat.RotateVector(FVector::ForwardVector);
	}
	else
	{
		return Dir.GetSafeNormal();
	}
	
}

void UBattleGameplayAbility_RangedWeapon::TraceBulletsInCartridge(const FRangedWeaponFiringInput& InputData,
                                                                  TArray<FHitResult>& OutHits)
{
	UBattleRangedWeaponInstance* WeaponData = InputData.WeaponData;
	check(WeaponData);

	const int32 BulletsPerCartridge = WeaponData->GetBulletsPerCartridge();

	for (int32 BulletIndex = 0; BulletIndex < BulletsPerCartridge; ++BulletIndex)
	{
		// Heat 정도에 따라서 정해진 현재 탄퍼짐 정도
		const float BaseSpreadAngle = WeaponData->GetCalculatedSpreadAngle();
		// 움직임 등 상황에 따라서 탄퍼짐 정도에 더해질 가중치 곱
		const float SpreadAngleMultiplier = WeaponData->GetCalculatedSpreadAngleMultiplier();
		// 현재 최대 탄퍼짐 각도 (Degree)
		const float ActualSpreadAngle = BaseSpreadAngle * SpreadAngleMultiplier;

		// 원뿔로 계산할 것이기에 Half에 대해서 라디안값 구함
		const float HalfSpreadAngleInRadians = FMath::DegreesToRadians(ActualSpreadAngle * 0.5f);

		// 탄퍼짐 정도에 따라서 랜덤성을 어느정도 부여하여 탄환의 방향을 정함
		const FVector BulletDir = VRandConeNormalDistribution(InputData.AimDir, HalfSpreadAngleInRadians, WeaponData->GetSpreadExponent());

		const FVector EndTrace = InputData.StartTrace + (BulletDir * WeaponData->GetMaxDamageRange());
		FVector HitLocation = EndTrace;

		TArray<FHitResult> AllImpacts;
		FHitResult Impact = DoSingleBulletTrace(InputData.StartTrace, EndTrace, WeaponData->GetBulletTraceSweepRadius(), false, AllImpacts);
		
		const AActor* HitActor = Impact.GetActor();
		if (HitActor)
		{
			if (AllImpacts.Num() >0)
			{
				OutHits.Append(AllImpacts);
			}

			HitLocation = Impact.ImpactPoint;
		}

		if (OutHits.Num() == 0)
		{
			if (!Impact.bBlockingHit)
			{
				Impact.Location = EndTrace;
				Impact.ImpactPoint = EndTrace;
			}
			OutHits.Add(Impact);
		}
	}

}

UBattleRangedWeaponInstance* UBattleGameplayAbility_RangedWeapon::GetWeaponInstance()
{
	return Cast<UBattleRangedWeaponInstance>(GetAssociatedEquipment());
}

FVector UBattleGameplayAbility_RangedWeapon::GetWeaponTargetingSourceLocation() const
{
	// 무기가 아닌 Pawn의 위치를 가져옴
	APawn* const AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());
	check(AvatarPawn);

	const FVector SourceLoc = AvatarPawn->GetActorLocation();
	return SourceLoc;
	
}

FTransform UBattleGameplayAbility_RangedWeapon::GetTargetingTransform(APawn* SourcePawn,
                                                                      EBattleAbilityTargetingSource Source)
{
	check(SourcePawn);
	check(Source == EBattleAbilityTargetingSource::CameraTowardsFocus);

	AController* Controller = SourcePawn->GetController();
	if (Controller == nullptr)
	{
		return FTransform();
	}

	double FocalDistance = 1024.0f;
	FVector FocalLoc;
	FVector CamLoc;
	FRotator CamRot;

	
	APlayerController* PC = Cast<APlayerController>(Controller);
	if (PC != nullptr)
	{
		PC->GetPlayerViewPoint(CamLoc, CamRot);
	}
	else
	{
		CamLoc = GetWeaponTargetingSourceLocation();
		CamRot = Controller->GetControlRotation();
	}
	
	FVector AimDir = CamRot.Vector().GetSafeNormal();
	FocalLoc = CamLoc + (AimDir*FocalDistance);

	FVector FinalCamLoc;

	if (PC)
	{
		const FVector WeaponLoc = GetWeaponTargetingSourceLocation();
		FinalCamLoc = FocalLoc + (((WeaponLoc - FocalLoc) | AimDir)*AimDir);
	}
	else
	{
		const FVector WeaponLoc = GetWeaponTargetingSourceLocation();
		CamLoc = SourcePawn->GetActorLocation() + FVector(0,0,SourcePawn->BaseEyeHeight);
		FocalLoc = CamLoc + (AimDir*FocalDistance);
		FinalCamLoc = FocalLoc + (((WeaponLoc - FocalLoc) | AimDir)*AimDir);
	}
#if 0
	{
		DrawDebugPoint(GetWorld(), WeaponLoc, 10.0f, FColor::Red, false, 60.0f);
		DrawDebugPoint(GetWorld(), CamLoc, 10.0f, FColor::Yellow, false, 60.0f);
		DrawDebugPoint(GetWorld(), FinalCamLoc, 10.0f, FColor::Magenta, false, 60.0f);

		DrawDebugLine(GetWorld(), FocalLoc, WeaponLoc, FColor::Yellow, false, 60.0f, 0, 2.0f);
		DrawDebugLine(GetWorld(), CamLoc, FocalLoc, FColor::Blue, false, 60.0f, 0, 2.0f);
		DrawDebugLine(GetWorld(), WeaponLoc, FinalCamLoc, FColor::Red, false, 60.0f, 0, 2.0f);
	}
#endif

	return FTransform(CamRot, FinalCamLoc);
	
}

void UBattleGameplayAbility_RangedWeapon::PerformLocalTargeting(TArray<FHitResult>& OutHits)
{
	APawn* const AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());

	UBattleRangedWeaponInstance* WeaponData = GetWeaponInstance();
	if ( AvatarPawn && AvatarPawn->IsLocallyControlled() && WeaponData)
	{
		FRangedWeaponFiringInput InputData;
		InputData.WeaponData = WeaponData;
		InputData.bCanPlayBulletFX = true;

		const FTransform TargetTransform = GetTargetingTransform(AvatarPawn, EBattleAbilityTargetingSource::CameraTowardsFocus);

		InputData.AimDir = TargetTransform.GetUnitAxis(EAxis::X);
		InputData.StartTrace = TargetTransform.GetTranslation();
		InputData.EndAim = InputData.StartTrace + InputData.AimDir*WeaponData->GetMaxDamageRange();
		
#if 0
		{
			static float DebugThickness = 2.0f;
			DrawDebugLine(GetWorld(), InputData.StartTrace, InputData.StartTrace + (InputData.AimDir + 100.0f), FColor::Yellow, false, 10.0f, 0, DebugThickness);
		}
#endif
#if 0
{
	static float DebugThickness = 2.0f;
	DrawDebugLine(GetWorld(), InputData.StartTrace, InputData.EndAim, FColor::Yellow, false, 10.0f, 0, DebugThickness);
}
#endif
		TraceBulletsInCartridge(InputData, OutHits);
	}
}

void UBattleGameplayAbility_RangedWeapon::OnTargetDataReadyCallback(const FGameplayAbilityTargetDataHandle& InData,
	FGameplayTag ApplicationTag)
{
	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
	check(ASC);

	if (const FGameplayAbilitySpec* AbilitySpec = ASC->FindAbilitySpecFromHandle(CurrentSpecHandle))
	{
		FGameplayAbilityTargetDataHandle LocalTargetDataHandle(MoveTemp(const_cast<FGameplayAbilityTargetDataHandle&>(InData)));

		if (CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo))
		{
			UBattleRangedWeaponInstance* WeaponData = GetWeaponInstance();
			check(WeaponData);
			WeaponData->AddSpared();
			
			OnRangeWeaponTargetDataReady(LocalTargetDataHandle);
		}
		else
		{
			K2_EndAbility();
		}
	}
	
	
}

void UBattleGameplayAbility_RangedWeapon::StartRangedWeaponTargeting()
{
	check(CurrentActorInfo);

	AActor* AvatarActor = CurrentActorInfo->AvatarActor.Get();
	check(AvatarActor);

	UAbilitySystemComponent* ASC = CurrentActorInfo->AbilitySystemComponent.Get();
	check(ASC);

	TArray<FHitResult> FoundHits;
	PerformLocalTargeting(FoundHits);

	FGameplayAbilityTargetDataHandle TargetData;
	TargetData.UniqueId = 0;

	if (FoundHits.Num() >0)
	{
		const int32 CartridgeID = FMath::Rand();
		for (const FHitResult& FoundHit : FoundHits)
		{
			FBattleGameplayAbilityTargetData_SingleTargetHit* NewTargetData = new FBattleGameplayAbilityTargetData_SingleTargetHit();
			NewTargetData->HitResult = FoundHit;
			NewTargetData->CartridgeID = CartridgeID;
			TargetData.Add(NewTargetData);
		}
	}

	OnTargetDataReadyCallback(TargetData, FGameplayTag());
	
	
	
}
