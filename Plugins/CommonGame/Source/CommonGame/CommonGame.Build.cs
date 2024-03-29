using UnrealBuildTool;

public class CommonGame : ModuleRules
{
    public CommonGame(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "InputCore",
                "Engine",
                "Slate",
                "SlateCore",
                "UMG",
                "CommonUI",
                "CommonUser",
                "GameplayTags",
                "ModularGameplayActors",
                
            }
        );
    }


}