#pragma once

#include "AbilitySystemInterface.h"
#include "ModularCharacter.h"
#include "BattleCharacter.generated.h"

class UBattleAbilitySystemComponent;
class UBattleHealthComponent;
class UBattleCameraComponent;
class UBattlePawnExtensionComponent;


UCLASS()
class ABattleCharacter : public AModularCharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()
public:
	ABattleCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="Battle|Character")
	UBattleAbilitySystemComponent* GetBattleAbilitySystemComponent() const;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	

	
	UFUNCTION()
	virtual void OnDeathStarted(AActor* OwningActor);

	UFUNCTION()
	virtual void OnDeathFinished(AActor* OwningActor);

	UFUNCTION(BlueprintImplementableEvent, meta=(DisplayName="OnDeathFinished"))
	void K2_OnDeathFinished();

	void DisableMovementAndCollision();
	void DestroyDueToDeath();
	void UninitAndDestroy();
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) final;
	
	virtual void Reset() override;

protected:

	void OnAbilitySystemInitialized();
	void OnAbilitySystemUninitialized();

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed() override;

	void InitializeGameplayTags();
	
private:

	// PawnExtensionComponent는 SubObject로 붙여둠.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Battle|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBattlePawnExtensionComponent> PawnExtComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Battle|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBattleCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Battle|Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBattleHealthComponent> HealthComponent;
	
};