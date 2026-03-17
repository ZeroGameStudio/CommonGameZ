// Copyright Zero Games. All Rights Reserved.

using UnrealBuildTool;

public class CommonGameZRuntime : ModuleRules
{
	public CommonGameZRuntime(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange
		(
			new string[]
			{
			}
		);
		
		PrivateIncludePaths.AddRange
		(
			new string[]
			{
			}
		);
		
		PublicDependencyModuleNames.AddRange
		(
			new string[]
			{
				"Core",
				"CoreUObject",
				"NetCore",
				"Engine",
				"GameplayTags",
				"AIModule",
				"EnhancedInput",
				
				"DeveloperSettings",
				"GameplayDebugger",
				
				"ZSharpCore",
				"ZSharpRuntime",
			
				"ZeroExtensionSystemRuntime",
			}
		);
		
		PrivateDependencyModuleNames.AddRange
		(
			new string[]
			{
				"ApplicationCore",
			}
		);
		
		DynamicallyLoadedModuleNames.AddRange
		(
			new string[]
			{
			}
		);
	}
}


