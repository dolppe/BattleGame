#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class FUIExtensionModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

void FUIExtensionModule::StartupModule()
{
	
}

void FUIExtensionModule::ShutdownModule()
{
	
}
	
IMPLEMENT_MODULE(FUIExtensionModule, UIExtension)