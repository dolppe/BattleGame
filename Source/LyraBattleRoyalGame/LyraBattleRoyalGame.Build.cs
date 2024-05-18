// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LyraBattleRoyalGame : ModuleRules
{
	public LyraBattleRoyalGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			// GAS
			"GameplayTags",
			"GameplayTasks",
			"GameplayAbilities",
			// Game Feature
			"ModularGameplay",
			"GameFeatures",
			"ModularGameplayActors",
			// Input
			"InputCore",
			"EnhancedInput",
			// CommonUser
			"CommonUser",
			// CommonGame
			"CommonGame",
			// CommonUI
			"CommonUI",
			// UMG
			"UMG",
			// UIExtension
			"UIExtension",
			// Slate
			"Slate",
			"SlateCore",
			// GameplayMessageRouter
			"GameplayMessageRuntime",
			// Physics
			"PhysicsCore",
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
