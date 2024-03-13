#include "BattleControllerComponent_CharacterParts.h"
#include "BattlePawnComponent_CharacterParts.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleControllerComponent_CharacterParts)

UBattleControllerComponent_CharacterParts::UBattleControllerComponent_CharacterParts(
	const FObjectInitializer& ObjectInitializer)
		: Super(ObjectInitializer)
{
}

void UBattleControllerComponent_CharacterParts::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		if (AController* OwningController = GetController<AController>())
		{
			OwningController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPossessedPawnChanged);
		}
	}
}

void UBattleControllerComponent_CharacterParts::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveAllCharacterParts();
	Super::EndPlay(EndPlayReason);
}

UBattlePawnComponent_CharacterParts* UBattleControllerComponent_CharacterParts::GetPawnCustomizer() const
{
	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		return ControlledPawn->FindComponentByClass<UBattlePawnComponent_CharacterParts>();
	}
	return nullptr;
}

void UBattleControllerComponent_CharacterParts::AddCharacterPartInternal(const FBattleCharacterPart& NewPart)
{
	FBattleControllerCharacterPartEntry& NewEntry = CharacterParts.AddDefaulted_GetRef();
	NewEntry.Part = NewPart;

	if (UBattlePawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		NewEntry.Handle = PawnCustomizer->AddCharacterPart(NewPart);
	}
}

void UBattleControllerComponent_CharacterParts::AddCharacterPart(const FBattleCharacterPart& NewPart)
{
	AddCharacterPartInternal(NewPart);
}

void UBattleControllerComponent_CharacterParts::RemoveAllCharacterParts()
{
	if (UBattlePawnComponent_CharacterParts* PawnCustomizer = GetPawnCustomizer())
	{
		for (FBattleControllerCharacterPartEntry& Entry : CharacterParts)
		{
			PawnCustomizer->RemoveCharacterPart(Entry.Handle);
		}
	}
	CharacterParts.Reset();
}

void UBattleControllerComponent_CharacterParts::OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	if (UBattlePawnComponent_CharacterParts* OldCustomizer = OldPawn ? OldPawn->FindComponentByClass<UBattlePawnComponent_CharacterParts>() : nullptr)
	{
		for (FBattleControllerCharacterPartEntry& Entry : CharacterParts)
		{
			OldCustomizer->RemoveCharacterPart(Entry.Handle);
			Entry.Handle.Reset();
		}
	}

	if (UBattlePawnComponent_CharacterParts* NewCustomizer = NewPawn ? NewPawn->FindComponentByClass<UBattlePawnComponent_CharacterParts>() : nullptr)
	{
		for (FBattleControllerCharacterPartEntry& Entry : CharacterParts)
		{
			check(!Entry.Handle.IsValid());
			Entry.Handle = NewCustomizer->AddCharacterPart(Entry.Part);
		}
	}
}
