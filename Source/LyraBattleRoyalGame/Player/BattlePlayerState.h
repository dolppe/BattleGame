#pragma once

#include "GameFramework/PlayerState.h"
#include "LyraBattleRoyalGame/Character/BattlePawnData.h"
#include "BattlePlayerState.generated.h"

class UBattleAbilitySystemComponent;
class UBattleExperienceDefinition;
class UBattlePawnData;

UCLASS()
class ABattlePlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	ABattlePlayerState(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	/*
	 *
	 * AActor's Interface 
	 */
	virtual void PostInitializeComponents() override;

	/*
	 *
	 * member methods
	 */
	
	template <class T>
	const T* GetPawnData() const {return Cast<T>(PawnData);}
	void SetPawnData(const UBattlePawnData* InPawnData);
	void OnExperienceLoaded(const UBattleExperienceDefinition* CurrentExperience);

	UBattleAbilitySystemComponent* GetBattleAbilitySystemComponent() const {return AbilitySystemComponent;}
	

	
private:

	// PawnData
	UPROPERTY()
	TObjectPtr<const UBattlePawnData> PawnData;

	UPROPERTY(VisibleAnywhere, Category="Battle|PlayerState")
	TObjectPtr<UBattleAbilitySystemComponent> AbilitySystemComponent;
	
};
