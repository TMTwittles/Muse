// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MuseRangeCombat : ModuleRules
{
	public MuseRangeCombat(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "MuseLockOn"});
	}
}
