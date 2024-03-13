#pragma once

#include "CoreMinimal.h"
#include "BattleCharacterPartTypes.generated.h"


USTRUCT(BlueprintType)
struct FBattleCharacterPartHandle
{
	GENERATED_BODY()

	void Reset()
	{
		PartHandle = INDEX_NONE;
	}

	bool IsValid() const
	{
		return PartHandle != INDEX_NONE;
	}

	UPROPERTY()
	int32 PartHandle = INDEX_NONE;
	
};

USTRUCT(BlueprintType)
struct FBattleCharacterPart
{
	GENERATED_BODY()

	// Actor Class Of Instanced Part
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> PartClass;

	// Attach BoneSocket Name
	// Root is Default
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;
	
};
