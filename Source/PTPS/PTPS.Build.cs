// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PTPS : ModuleRules
{
	public PTPS(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "NavigationSystem", "AIModule", "GameplayTasks", "Niagara", "GameplayTags"});
	}
}