#pragma once

#include "GameFramework/Character.h"
#include "BattleCharacter.generated.h"

class UBattlePawnExtensionComponent;

UCLASS()
class ABattleCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	ABattleCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) final;
	
private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Battle|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBattlePawnExtensionComponent> PawnExtComponent;
	
};