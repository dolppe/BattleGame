#pragma once

#include "Factories/Factory.h"
#include "UObject/NameTypes.h"
#include "UObject/UObjectGlobals.h"

#include "BattleContextEffectsLibraryFactory.generated.h"

class UObject;
class UClass;
class FFeedbackContext;

UCLASS(hidecategories = Object, MinimalAPI)
class UBattleContextEffectsLibraryFactory : public UFactory
{
	GENERATED_BODY()
public:

	UBattleContextEffectsLibraryFactory(const FObjectInitializer& ObjectInitializer);

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;

	virtual bool ShouldShowInNewMenu() const override
	{
		return true;
	}
	
};
