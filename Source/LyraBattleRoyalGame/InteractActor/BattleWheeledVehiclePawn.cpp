#include "BattleWheeledVehiclePawn.h"

#include "GameplayMessageSubsystem.h"
#include "NativeGameplayTags.h"
#include "LyraBattleRoyalGame/Character/BattleCharacter.h"
#include "LyraBattleRoyalGame/Character/BattleHeroComponent.h"
#include "LyraBattleRoyalGame/Interaction/BattleInteractionStringMessage.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleWheeledVehiclePawn)

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_SHOOTERGAME_INTERACTION_MESSAGE_START, "ShooterGame.Interaction.Message.Start");
UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_SHOOTERGAME_INTERACTION_MESSAGE_END, "ShooterGame.Interaction.Message.End");


ABattleWheeledVehiclePawn::ABattleWheeledVehiclePawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void ABattleWheeledVehiclePawn::StartInteraction_Implementation(AController* InController)
{
	// 문구 보냄 (태그 => 플레이어랑 문구)
	// 플레이어, 로컬 플레이어 인 경우에만

	if (APlayerController* PC = Cast<APlayerController>(InController))
	{
		if (PC->IsLocalController())
		{
			FBattleInteractionStringMessage Message;
			Message.Target = Cast<AActor>(PC);
			Message.MessageString = TEXT("Boarding Vehicle");

			UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(GetWorld());
			MessageSubsystem.BroadcastMessage(TAG_SHOOTERGAME_INTERACTION_MESSAGE_START, Message);

			if (ABattleCharacter* BattleCharacter = Cast<ABattleCharacter>(PC->GetPawn()))
			{
				if (UBattleHeroComponent* HeroComp = BattleCharacter->FindComponentByClass<UBattleHeroComponent>())
				{
					HeroComp->SetInteraction(this);
				}
			}
		}
	}

	
	
}


void ABattleWheeledVehiclePawn::EndInteraction_Implementation(AController* InController)
{
	if (APlayerController* PC = Cast<APlayerController>(InController))
	{
		if (PC->IsLocalController())
		{
			FBattleInteractionStringMessage Message;
			Message.Target = Cast<AActor>(PC);
			Message.MessageString = TEXT("Boarding Vehicle");

			UGameplayMessageSubsystem& MessageSubsystem = UGameplayMessageSubsystem::Get(GetWorld());
			MessageSubsystem.BroadcastMessage(TAG_SHOOTERGAME_INTERACTION_MESSAGE_END, Message);
		}
	}
}
