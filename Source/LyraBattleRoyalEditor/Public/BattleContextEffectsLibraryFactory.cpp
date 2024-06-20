#include "BattleContextEffectsLibraryFactory.h"

#include "../../LyraBattleRoyalGame/Feedback/ContextEffects/BattleContextEffectsLibrary.h"
#include "Templates/SubclassOf.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BattleContextEffectsLibraryFactory)

UBattleContextEffectsLibraryFactory::UBattleContextEffectsLibraryFactory(const FObjectInitializer& ObjectInitializer)
{
	SupportedClass = UBattleContextEffectsLibrary::StaticClass();

	bCreateNew = true;
	bEditorImport = false;
	bEditAfterNew = true;
}

UObject* UBattleContextEffectsLibraryFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UBattleContextEffectsLibrary* BattleContextEffectsLibrary = NewObject<UBattleContextEffectsLibrary>(InParent, Name, Flags);

	return BattleContextEffectsLibrary;
}
