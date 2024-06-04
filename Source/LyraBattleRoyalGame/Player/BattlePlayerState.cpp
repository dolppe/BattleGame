#include "BattlePlayerState.h"

#include "LyraBattleRoyalGame/BattleLogChannels.h"
#include "LyraBattleRoyalGame/GameModes/BattleExperienceManagerComponent.h"
#include "LyraBattleRoyalGame/GameModes/BattleGameMode.h"
#include "LyraBattleRoyalGame/AbilitySystem/BattleAbilitySystemComponent.h"
#include "LyraBattleRoyalGame/AbilitySystem/BattleAbilitySet.h"
#include "LyraBattleRoyalGame/AbilitySystem/Attributes/BattleCombatSet.h"
#include "LyraBattleRoyalGame/AbilitySystem/Attributes/BattleHealthSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattlePlayerState)

ABattlePlayerState::ABattlePlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<UBattleAbilitySystemComponent>(this, TEXT("AbilitySystemcomponent"));

	
	CreateDefaultSubobject<UBattleHealthSet>(TEXT("HealthSet"));
	CreateDefaultSubobject<UBattleCombatSet>(TEXT("CombatSet"));
}

void ABattlePlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	{
		FGameplayAbilityActorInfo* ActorInfo = AbilitySystemComponent->AbilityActorInfo.Get();
		check(ActorInfo->OwnerActor == this);
		check(ActorInfo->OwnerActor == ActorInfo->AvatarActor);		
	}
	// 아직 Pawn이 안붙었기에 nullptr이긴 함. 초기화 느낌으로 nullptr로 초기화.
	AbilitySystemComponent->InitAbilityActorInfo(this, GetPawn());
	
	
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

	for (UBattleAbilitySet* AbilitySet : PawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}
	
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

void ABattlePlayerState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	const UBattleHealthSet* temp = AbilitySystemComponent->GetSet<UBattleHealthSet>();

	UE_LOG(LogBattle, Log, TEXT("Health: %f"),temp->GetHealth());
}

UAbilitySystemComponent* ABattlePlayerState::GetAbilitySystemComponent() const
{
	return GetBattleAbilitySystemComponent();
	
}

PRAGMA_DISABLE_OPTIMIZATION

void ABattlePlayerState::AddStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.AddStack(Tag, StackCount);
}

void ABattlePlayerState::RemoveStatTagStack(FGameplayTag Tag, int32 StackCount)
{
	StatTags.RemoveStack(Tag, StackCount);
}

int32 ABattlePlayerState::GetStatTagStack(FGameplayTag Tag)
{
	return StatTags.GetStackCount(Tag);
}

bool ABattlePlayerState::HasStatTag(FGameplayTag Tag) const
{
	return StatTags.ContainsTag(Tag);
}

PRAGMA_ENABLE_OPTIMIZATION