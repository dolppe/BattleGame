#pragma once

#include "Engine/DataAsset.h"
#include "BattlePawnData.generated.h"

UCLASS()
class UBattlePawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UBattlePawnData(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Battle|Pawn")
	TSubclassOf<APawn> PawnClass;
	
};
