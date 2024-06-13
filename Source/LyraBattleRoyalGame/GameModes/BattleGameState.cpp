#include "BattleGameState.h"

#include "BattleExperienceManagerComponent.h"
#include "GameplayMessageSubsystem.h"
#include "LyraBattleRoyalGame/Messages/BattleVerbMessage.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleGameState)

ABattleGameState::ABattleGameState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ExperienceManagerComponent = CreateDefaultSubobject<UBattleExperienceManagerComponent>(TEXT("ExperneceManagerComponent"));

	AbilitySystemComponent = CreateDefaultSubobject<UBattleAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UAbilitySystemComponent* ABattleGameState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ABattleGameState::MulticastReliableMessageToClients_Implementation(const FBattleVerbMessage Message)
{
	if (GetNetMode() == NM_Client)
	{
		UGameplayMessageSubsystem::Get(this).BroadcastMessage(Message.Verb, Message);
	}
}

void ABattleGameState::MulticastMessageToClients_Implementation(const FBattleVerbMessage Message)
{
	MulticastMessageToClients_Implementation(Message);
}
