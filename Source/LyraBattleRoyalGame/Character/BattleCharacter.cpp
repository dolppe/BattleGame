#include "BattleCharacter.h"
#include "BattlePawnExtensionComponent.h"
#include "LyraBattleRoyalGame/Camera/BattleCameraComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleCharacter)


ABattleCharacter::ABattleCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// PawnExtComponent 생성
	PawnExtComponent = CreateDefaultSubobject<UBattlePawnExtensionComponent>(TEXT("PawnExtensionComponent"));

	// CameraComponent 생성
	{
		CameraComponent = CreateDefaultSubobject<UBattleCameraComponent>(TEXT("CameraComponent"));
		CameraComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 75.0f));
	}
	
}

// PC가 Possess한 이후에 실행되는데 이때 PawnExtensionComponent의 InitState 초기화를 연결시켜줌.
void ABattleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnExtComponent->SetupPlayerInputComponent();
}
