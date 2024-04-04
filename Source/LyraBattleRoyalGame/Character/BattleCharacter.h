#pragma once

#include "AbilitySystemInterface.h"
#include "ModularCharacter.h"
#include "BattleCharacter.generated.h"

class UBattleHealthComponent;
class UBattleCameraComponent;
class UBattlePawnExtensionComponent;


UCLASS()
class ABattleCharacter : public AModularCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ABattleCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void OnAbilitySystemInitialized();
	void OnAbilitySystemUninitialized();
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) final;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
private:

	// PawnExtensionComponent는 SubObject로 붙여둠.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Battle|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBattlePawnExtensionComponent> PawnExtComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Battle|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBattleCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Battle|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBattleHealthComponent> HealthComponent;
	
};