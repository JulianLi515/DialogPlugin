// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DialogPlugin : ModuleRules
{
	public DialogPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "DialogEditorRuntime" });

		if (Target.bBuildEditor)
		{
			PublicDependencyModuleNames.AddRange(new string[]{"DialogEditor"});
		}
	}
}
