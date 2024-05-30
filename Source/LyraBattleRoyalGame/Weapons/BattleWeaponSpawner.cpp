#include "BattleWeaponSpawner.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LyraBattleRoyalGame/BattleLogChannels.h"
#include "LyraBattleRoyalGame/Equipment/BattlePickupDefinition.h"
#include "LyraBattleRoyalGame/Inventory/BattleInventoryFragment_SetStats.h"
#include "LyraBattleRoyalGame/Inventory/BattleInventoryItemDefinition.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleWeaponSpawner)

ABattleWeaponSpawner::ABattleWeaponSpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionVolume = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionVolume"));
	RootComponent = CollisionVolume;
	CollisionVolume->InitCapsuleSize(80.f, 80.f);
	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnOverlapBegin);

	PadMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadMesh"));
	PadMesh->SetupAttachment(RootComponent);

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);

	WeaponMeshRotationSpeed = 40.0f;
	CoolDownTime = 30.0f;
	CheckExistingOverlapDelay = 0.25f;
	bIsWeaponAvailable = true;
	
}

void ABattleWeaponSpawner::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UWorld* World = GetWorld();
	if (World->GetTimerManager().IsTimerActive(CoolDownTimerHandle))
	{
		CoolDownPercentage = 1.0f - World->GetTimerManager().GetTimerRemaining(CoolDownTimerHandle)/CoolDownTime;
	}

	WeaponMesh->AddRelativeRotation(FRotator(0.0f, World->GetDeltaSeconds()*WeaponMeshRotationSpeed, 0.0f));
}

void ABattleWeaponSpawner::OnConstruction(const FTransform& Transform)
{
	if (WeaponDefinition != nullptr && WeaponDefinition->DisplayMesh != nullptr)
	{
		WeaponMesh->SetStaticMesh(WeaponDefinition->DisplayMesh);
		WeaponMesh->SetRelativeLocation(WeaponDefinition->WeaponMeshOffset);
		WeaponMesh->SetRelativeScale3D(WeaponDefinition->WeaponMeshScale);
	}
}

void ABattleWeaponSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponDefinition && WeaponDefinition->InventoryItemDefinition)
	{
		CoolDownTime = WeaponDefinition->SpawnCoolDownSeconds;
	}
	else
	{
		UE_LOG(LogBattle, Error, TEXT("WeaponSpawner: Invalid Weapon Defintion"));
	}
}

void ABattleWeaponSpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(CoolDownTimerHandle);
		World->GetTimerManager().ClearTimer(CheckOverlapsDelayTimerHandle);
	}

	Super::EndPlay(EndPlayReason);
	
}

void ABattleWeaponSpawner::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	APawn* OverlappingPawn = Cast<APawn>(OtherActor);
	if (GetLocalRole() == ROLE_Authority && bIsWeaponAvailable && OverlappingPawn != nullptr)
	{
		AttemptPickUpWeapon(OverlappingPawn);
	}
	
}

void ABattleWeaponSpawner::CheckForExistingOverlaps()
{
	TArray<AActor*> OverlappingActors;
	GetOverlappingActors(OverlappingActors, APawn::StaticClass());

	for (AActor* OverlappingActor : OverlappingActors)
	{
		AttemptPickUpWeapon(Cast<APawn>(OverlappingActor));
	}
}

void ABattleWeaponSpawner::SetWeaponPickupVisibility(bool bShouldBeVisible)
{
	WeaponMesh->SetVisibility(bShouldBeVisible, true);
}

void ABattleWeaponSpawner::OnCoolDownTimerComplete()
{
	ResetCoolDown();
}

void ABattleWeaponSpawner::StartCoolDown()
{
	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().SetTimer(CoolDownTimerHandle, this, &ThisClass::OnCoolDownTimerComplete, CoolDownTime);
	}
}

void ABattleWeaponSpawner::ResetCoolDown()
{
	UWorld* World = GetWorld();

	if (World)
	{
		World->GetTimerManager().ClearTimer(CoolDownTimerHandle);
	}

	if (GetLocalRole() == ROLE_Authority)
	{
		bIsWeaponAvailable = true;
		PlayRespawnEffects();
		SetWeaponPickupVisibility(true);

		if (World)
		{
			World->GetTimerManager().SetTimer(CheckOverlapsDelayTimerHandle, this,&ThisClass::CheckForExistingOverlaps, CheckExistingOverlapDelay);
		}
	}

	CoolDownPercentage =0.0f;
}
int32 ABattleWeaponSpawner::GetDefaultStatFromItemDef(const TSubclassOf<UBattleInventoryItemDefinition> WeaponItemClass,
	FGameplayTag StatTag)
{
	if (WeaponItemClass != nullptr)
	{
		if (UBattleInventoryItemDefinition* WeaponItemCDO = WeaponItemClass->GetDefaultObject<UBattleInventoryItemDefinition>())
		{
			if (const UBattleInventoryFragment_SetStats* ItemStatsFragment = Cast<UBattleInventoryFragment_SetStats>(WeaponItemCDO->FindFragmentByClass(UBattleInventoryFragment_SetStats::StaticClass())))
			{
				return ItemStatsFragment->GetItemStatByTag(StatTag);				
			}
		}
	}
	return 0;
}

void ABattleWeaponSpawner::PlayRespawnEffects_Implementation()
{
	if (WeaponDefinition != nullptr)
	{
		USoundBase* PickupSound = WeaponDefinition->PickedUpSound;
		if (PickupSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
		}

		UNiagaraSystem* PickupEffect = WeaponDefinition->PickedUpEffect;
		if (PickupEffect != nullptr)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PickupEffect, WeaponMesh->GetComponentLocation());
		}
	}
}

void ABattleWeaponSpawner::PlayPickupEffects_Implementation()
{
	if (WeaponDefinition != nullptr)
	{
		USoundBase* RespawnSound = WeaponDefinition->RespawnedSound;
		if (RespawnSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, RespawnSound, GetActorLocation());
		}

		UNiagaraSystem* RespawnEffect = WeaponDefinition->RespawnedEffect;
		if (RespawnEffect != nullptr)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, RespawnEffect, WeaponMesh->GetComponentLocation());
		}
	}
}

void ABattleWeaponSpawner::AttemptPickUpWeapon_Implementation(APawn* Pawn)
{
	if (GetLocalRole() == ROLE_Authority && bIsWeaponAvailable && UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Pawn))
	{
		TSubclassOf<UBattleInventoryItemDefinition> WeaponItemDefinition = WeaponDefinition ? WeaponDefinition->InventoryItemDefinition : nullptr;
		if (WeaponItemDefinition != nullptr)
		{
			if (GiveWeapon(WeaponItemDefinition, Pawn))
			{
				bIsWeaponAvailable = false;

				SetWeaponPickupVisibility(false);
				PlayPickupEffects();
				StartCoolDown();
			}
		}
	}
}
