#include "BattleCharacter.h"
#include "BattlePawnExtensionComponent.h"
#include "LyraBattleRoyalGame/AbilitySystem/BattleAbilitySystemComponent.h"
#include "LyraBattleRoyalGame/Camera/BattleCameraComponent.h"
#include "BattleHealthComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleCharacter)


ABattleCharacter::ABattleCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->InitCapsuleSize(40.0f, 90.0f);
	
	// PawnExtComponent 생성
	PawnExtComponent = CreateDefaultSubobject<UBattlePawnExtensionComponent>(TEXT("PawnExtensionComponent"));
	{
		PawnExtComponent->OnAbilitySystemInitialized_RegisterAndCall(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemInitialized));
		PawnExtComponent->OnAbilitySystemUninitialized_Register(FSimpleMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAbilitySystemUninitialized));
	}
	// CameraComponent 생성
	{
		CameraComponent = CreateDefaultSubobject<UBattleCameraComponent>(TEXT("CameraComponent"));
		CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));
	}

	{
		HealthComponent = CreateDefaultSubobject<UBattleHealthComponent>(TEXT("HealthComponent"));
		HealthComponent->OnDeathStarted.AddDynamic(this, &ThisClass::OnDeathStarted);
		HealthComponent->OnDeathFinished.AddDynamic(this, &ThisClass::OnDeathFinished);
	}

	{
		UCharacterMovementComponent* MoveComp = GetCharacterMovement();
		MoveComp->GravityScale = 1.0f;
		MoveComp->MaxAcceleration = 2400.0f;
		MoveComp->BrakingFrictionFactor = 1.0f;
		MoveComp->BrakingFriction = 6.0f;
		MoveComp->GroundFriction = 8.0f;
		MoveComp->BrakingDecelerationWalking = 1400.0f;
		MoveComp->bUseControllerDesiredRotation = false;
		MoveComp->bOrientRotationToMovement = false;
		MoveComp->GetNavAgentPropertiesRef().bCanCrouch = true;
		MoveComp->bCanWalkOffLedgesWhenCrouching = true;
		MoveComp->SetCrouchedHalfHeight(65.0f);
	}

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	BaseEyeHeight = 80.0f;
	CrouchedEyeHeight = 50.0f;
}

UBattleAbilitySystemComponent* ABattleCharacter::GetBattleAbilitySystemComponent() const
{
	return Cast<UBattleAbilitySystemComponent>(GetAbilitySystemComponent());
}

void ABattleCharacter::OnAbilitySystemInitialized()
{
	UBattleAbilitySystemComponent* BattleASC = Cast<UBattleAbilitySystemComponent>(GetAbilitySystemComponent());
	check(BattleASC);

	HealthComponent->InitializeWithAbilitySystem(BattleASC);

	InitializeGameplayTags();
}

void ABattleCharacter::OnAbilitySystemUninitialized()
{
	HealthComponent->UnInitializeWithAbilitySystem();
}

void ABattleCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PawnExtComponent->HandleControllerChanged();
}

void ABattleCharacter::UnPossessed()
{
	Super::UnPossessed();

	PawnExtComponent->HandleControllerChanged();
}

void ABattleCharacter::InitializeGameplayTags()
{
}

void ABattleCharacter::OnDeathStarted(AActor* OwningActor)
{
	DisableMovementAndCollision();
}

void ABattleCharacter::OnDeathFinished(AActor* OwningActor)
{
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::DestroyDueToDeath);
}

void ABattleCharacter::DisableMovementAndCollision()
{
	if (Controller)
	{
		Controller->SetIgnoreMoveInput(true);
	}

	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	check(CapsuleComp);
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	MovementComponent->StopMovementImmediately();
	MovementComponent->DisableMovement();
	
}

void ABattleCharacter::DestroyDueToDeath()
{
	K2_OnDeathFinished();

	UninitAndDestroy();
}

void ABattleCharacter::UninitAndDestroy()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		DetachFromControllerPendingDestroy();
		SetLifeSpan(0.1f);
	}

	if (UBattleAbilitySystemComponent* BattleASC = GetBattleAbilitySystemComponent())
	{
		if (BattleASC->GetAvatarActor() == this)
		{
			PawnExtComponent->UnInitializeAbilitySystem();
		}
	}

	SetActorHiddenInGame(true);
}

// PC가 Possess한 이후에 실행되는데 이때 PawnExtensionComponent의 InitState 초기화를 연결시켜줌.
void ABattleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnExtComponent->SetupPlayerInputComponent();
}

void ABattleCharacter::Reset()
{
	DisableMovementAndCollision();

	K2_OnReset();

	UninitAndDestroy();
}

UAbilitySystemComponent* ABattleCharacter::GetAbilitySystemComponent() const
{
	return PawnExtComponent->GetAbilitySystemComponent();
}

void ABattleCharacter::ToggleCrouch()
{
	if (bIsCrouched || GetCharacterMovement()->bWantsToCrouch)
	{
		UnCrouch();
	}
	else if (GetCharacterMovement()->IsMovingOnGround())
	{
		Crouch();
	}
}
