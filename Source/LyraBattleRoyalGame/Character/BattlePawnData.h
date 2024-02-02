#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LyraBattleRoyalGame/Camera/BattleCameraMode.h"
#include "BattlePawnData.generated.h"

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

};
