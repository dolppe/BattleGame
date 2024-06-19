#include "AssetTypeActions_BattleContextEffectsLibrary.h"

#include "../../LyraBattleRoyalGame/Feedback/ContextEffects/BattleContextEffectsLibrary.h"

#define LOCTEXT_NAMESPACE "AssetTypeActions"

UClass* FAssetTypeActions_BattleContextEffectsLibrary::GetSupportedClass() const
{
	return UBattleContextEffectsLibrary::StaticClass();
}

#undef LOCTEXT_NAMESPACE