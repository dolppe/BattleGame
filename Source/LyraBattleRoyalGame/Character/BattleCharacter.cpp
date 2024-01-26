#include "BattleCharacter.h"
#include "BattlePawnExtensionComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleCharacter)


ABattleCharacter::ABattleCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	PawnExtComponent = CreateDefaultSubobject<UBattlePawnExtensionComponent>(TEXT("PawnExtensionComponent"));
	
}

// PC가 Possess한 이후에 실행되는데 이때 PawnExtensionComponent의 InitState 초기화를 연결시켜줌.
void ABattleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PawnExtComponent->SetupPlayerInputComponent();
}
