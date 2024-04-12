// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectArk : ModuleRules
{
	public ProjectArk(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "NavigationSystem", "AIModule", "Niagara", "UMG", "Paper2D" });

		PrivateDependencyModuleNames.AddRange(new string[] { });
	}
}
