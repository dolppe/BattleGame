#pragma once

#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "ModularCharacter.h"
#include "BattleCharacter.generated.h"

class UBattleCameraComponent;
class UBattlePawnExtensionComponent;
class UAbilitySystemComponent;

UCLASS()
class ABattleCharacter : public AModularCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ABattleCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) final;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
private:

	// PawnExtensionComponent는 SubObject로 붙여둠.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Battle|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBattlePawnExtensionComponent> PawnExtComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Battle|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBattleCameraComponent> CameraComponent;
	
};