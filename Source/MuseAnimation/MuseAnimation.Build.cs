// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MuseAnimation : ModuleRules
{
	public MuseAnimation(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine"});
	}
}
