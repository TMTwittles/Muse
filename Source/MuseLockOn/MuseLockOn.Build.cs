// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MuseLockOn : ModuleRules
{
	public MuseLockOn(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "MuseAnimation"});
	}
}
