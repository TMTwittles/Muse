// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MuseMovement : ModuleRules
{
	public MuseMovement(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine"});
	}
}
