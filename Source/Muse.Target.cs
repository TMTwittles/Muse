// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class MuseTarget : TargetRules
{
	public MuseTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("Muse");
    ExtraModuleNames.Add("MuseGAS");
    ExtraModuleNames.Add("MuseMeleeAttacks");
    ExtraModuleNames.Add("MuseMovement");
    ExtraModuleNames.Add("MuseAI");
	}
}
