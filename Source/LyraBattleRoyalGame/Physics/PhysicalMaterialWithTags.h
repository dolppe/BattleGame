#pragma once

#include "GameplayTagContainer.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "UObject/UObjectGlobals.h"

#include "PhysicalMaterialWithTags.generated.h"

class UObject;

UCLASS()
class UPhysicalMaterialWithTags : public UPhysicalMaterial
{
	GENERATED_BODY()
public:
	UPhysicalMaterialWithTags(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=PhysicalProperties)
	FGameplayTagContainer Tags;
	
};

