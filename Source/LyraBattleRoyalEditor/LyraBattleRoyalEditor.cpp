
#include "LyraBattleRoyalEditor.h"

#include "AssetToolsModule.h"
#include "AssetTypeActions_BattleContextEffectsLibrary.h"
#include "IAssetTools.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class FLyraBattleRoyalEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;
};

void FLyraBattleRoyalEditorModule::StartupModule()
{
	IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();

	AssetTools.RegisterAssetTypeActions(MakeShareable(new FAssetTypeActions_BattleContextEffectsLibrary));
}

void FLyraBattleRoyalEditorModule::ShutdownModule()
{
	
}
	
IMPLEMENT_GAME_MODULE(FLyraBattleRoyalEditorModule, LyraBattleRoyalEditor)