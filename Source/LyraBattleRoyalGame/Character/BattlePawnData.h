#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BattlePawnData.generated.h"

class UBattleAbilitySet;
class UBattleCameraMode;
class UBattleInputConfig;
class UBattleAbilityTagRelationshipMapping;

UCLASS()
class UBattlePawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UBattlePawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Battle|Pawn")
	TSubclassOf<APawn> PawnClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Battle|Camera")
	TSubclassOf<UBattleCameraMode> DefaultCameraMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Battle|InputConfig")
	TObjectPtr<UBattleInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Battle|Abilities")
	TArray<TObjectPtr<UBattleAbilitySet>> AbilitySets;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Battle|Abilities")
	TObjectPtr<UBattleAbilityTagRelationshipMapping> TagRelationshipMapping;

};
