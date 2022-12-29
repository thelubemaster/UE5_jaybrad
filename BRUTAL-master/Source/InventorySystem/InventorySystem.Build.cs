// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;
public class InventorySystem : ModuleRules
{
	public InventorySystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;


        //string ProjectRoot = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../"));
        //string PluginDir = Path.Combine(ProjectRoot, "Plugins/Runtime");
        //PublicIncludePaths.Add(PluginDir);
        string ProjectRoot1 = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../"));
        string PluginDir1 = Path.Combine(ProjectRoot1, "Source/InventorySystem/UI");
        PublicIncludePaths.Add(PluginDir1);
        string ProjectRoot2 = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../"));
        string PluginDir2 = Path.Combine(ProjectRoot2, "Source/InventorySystem/Util");
        PublicIncludePaths.Add(PluginDir2);
        string ProjectRoot3 = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../"));
        string PluginDir3 = Path.Combine(ProjectRoot3, "Source/InventorySystem/Vehicles");
        PublicIncludePaths.Add(PluginDir3);
        string ProjectRoot4 = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../"));
        string PluginDir4 = Path.Combine(ProjectRoot4, "Source/InventorySystem/Character");
        PublicIncludePaths.Add(PluginDir4);
        string ProjectRoot5 = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../"));
        string PluginDir5 = Path.Combine(ProjectRoot5, "Source/InventorySystem/Camera");
        PublicIncludePaths.Add(PluginDir5);
        string ProjectRoot6 = Path.GetFullPath(Path.Combine(ModuleDirectory, "../../"));
        string PluginDir6 = Path.Combine(ProjectRoot6, "Source/InventorySystem/Game");
        PublicIncludePaths.Add(PluginDir6);
        PublicDependencyModuleNames.AddRange(new string[] { 
			"Core",
			"CoreUObject",
			"Engine", 
			"InputCore",
            "Slate",
            "NetCore",
            "SlateCore",
			"OnlineSubsystem",
            "OnlineSubsystemUtils",
            "CinematicCamera",
            "OnlineSubsystemEOS",
			"Networking",
			"Sockets",
            "Foliage",
            "Niagara",
            "EnhancedInput",
             "MotionWarping",
             "ContextualAnimation",
             "Media",
             "MediaUtils",
             "MediaAssets",
             "AudioMixer",
            "GeometryCollectionEngine",
            "AnimToTexture",
            "ModularGameplay",
            "NetworkPrediction",
            "ZoneGraph",
            "AnimGraphRuntime",
            "DeveloperSettings",
            "RenderCore",
            "MoviePlayer",
            "Gauntlet",
            "ChaosVehiclesCore", 
			"ChaosVehicles" });

		PrivateDependencyModuleNames.AddRange(new string[] { });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
