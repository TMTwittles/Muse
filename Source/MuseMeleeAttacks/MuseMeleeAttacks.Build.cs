// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MuseMeleeAttacks : ModuleRules
{
	public MuseMeleeAttacks(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine"});
	}
}
