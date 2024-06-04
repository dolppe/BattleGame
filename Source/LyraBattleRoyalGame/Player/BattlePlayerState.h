#pragma once

#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "LyraBattleRoyalGame/Character/BattlePawnData.h"
#include "LyraBattleRoyalGame/System/BattleGameplayTagStack.h"
#include "BattlePlayerState.generated.h"


class UBattleAbilitySystemComponent;
class UBattleExperienceDefinition;
class UBattlePawnData;

UCLASS()
class ABattlePlayerState : public APlayerState, public IAbilitySystemInterface
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
	virtual void Tick(float DeltaSeconds) override;

	UBattleAbilitySystemComponent* GetBattleAbilitySystemComponent() const {return AbilitySystemComponent;}
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly,Category=Score)
	void AddStatTagStack(FGameplayTag Tag,int32 StackCount);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly,Category=Score)
	void RemoveStatTagStack(FGameplayTag Tag,int32 StackCount);

	UFUNCTION(BlueprintCallable, Category=Score)
	int32 GetStatTagStack(FGameplayTag Tag);

	UFUNCTION(BlueprintCallable, Category=Score)
	bool HasStatTag(FGameplayTag Tag) const;

	
	
protected:

	// PawnData
	UPROPERTY()
	TObjectPtr<const UBattlePawnData> PawnData;
	
private:
	
	UPROPERTY(VisibleAnywhere, Category="Battle|PlayerState")
	TObjectPtr<UBattleAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	FBattleGameplayTagStackContainer StatTags;
	
};
