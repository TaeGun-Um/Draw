// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DrawSystem : ModuleRules
{
	public DrawSystem(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.Add("DrawSystem");

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
