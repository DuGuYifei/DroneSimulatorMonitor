// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FlightSimulator : ModuleRules
{
	public FlightSimulator(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Sockets", "Networking", "CesiumRuntime", "Json", "AirSim" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
        CppStandard = CppStandardVersion.Cpp17;
        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
