#pragma once

#include "Components/ControllerComponent.h"
#include "BattleCharacterPartTypes.h"
#include "BattleControllerComponent_CharacterParts.generated.h"

class UBattlePawnComponent_CharacterParts;

// ControllerComponent own CharacterParts 

USTRUCT()
struct FBattleControllerCharacterPartEntry
{
	GENERATED_BODY()

	// Character Part Definition 
	UPROPERTY(EditAnywhere)
	FBattleCharacterPart Part;

	// Character Part Handle
	// Handle or Character Part Instanced by Pawn 
	FBattleCharacterPartHandle Handle;
	
};

/*
 *	When Controller Possess Pawn, Pawn Will Generate Cosmetic Actor(CharacterParts)
 *
 */

UCLASS(meta = (BlueprintSpawnableComponent))
class UBattleControllerComponent_CharacterParts : public UControllerComponent
{
	GENERATED_BODY()
public:
	UBattleControllerComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UBattlePawnComponent_CharacterParts* GetPawnCustomizer() const;
	void AddCharacterPartInternal(const FBattleCharacterPart& NewPart);
	
	UFUNCTION(BlueprintCallable, Category=Cosmetics)
	void AddCharacterPart(const FBattleCharacterPart& NewPart);

	void RemoveAllCharacterParts();

	UFUNCTION()
	void OnPossessedPawnChanged(APawn* OldPawn, APawn* NewPawn);

	UPROPERTY(EditAnywhere, Category=Cosmetics)
	TArray<FBattleControllerCharacterPartEntry> CharacterParts;
	
};

