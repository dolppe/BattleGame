#include "BattlePlayerState.h"

#include "LyraBattleRoyalGame/GameModes/BattleExperienceManagerComponent.h"
#include "LyraBattleRoyalGame/GameModes/BattleGameMode.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattlePlayerState)

ABattlePlayerState::ABattlePlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

void ABattlePlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AGameStateBase* GameState = GetWorld()->GetGameState();
	check(GameState);

	UBattleExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UBattleExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	// Experience가 로드되면 PawnData를 설정하기 위해 델리게이트 걸어둠.
	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnBattleExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
	
	
}

void ABattlePlayerState::SetPawnData(const UBattlePawnData* InPawnData)
{
	check(InPawnData);

	// PawnData가 두번 생성되는 것을 막음.
	check(!PawnData);
	
	PawnData = InPawnData;
}

void ABattlePlayerState::OnExperienceLoaded(const UBattleExperienceDefinition* CurrentExperience)
{
	if (ABattleGameMode* GameMode = GetWorld()->GetAuthGameMode<ABattleGameMode>())
	{
		
		const UBattlePawnData* NewPawnData = GameMode->GetPawnDataForController(GetOwningController());
		check(NewPawnData);

		SetPawnData(NewPawnData);
	}
}
