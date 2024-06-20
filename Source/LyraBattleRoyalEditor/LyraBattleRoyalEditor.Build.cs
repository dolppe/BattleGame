
using UnrealBuildTool;

public class LyraBattleRoyalEditor : ModuleRules
{
    public LyraBattleRoyalEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]{
                "Core",
                "CoreUObject",
                "Engine",
                "EditorFramework",
                "PhysicsCore",
                "LyraBattleRoyalGame",
                "GameplayTagsEditor",
                "DeveloperToolSettings",
                "UnrealEd",
                "GameplayTasksEditor",
                "GameplayAbilities",
                "GameplayAbilitiesEditor",
                
            }
        );
        
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                
            });
    }


}