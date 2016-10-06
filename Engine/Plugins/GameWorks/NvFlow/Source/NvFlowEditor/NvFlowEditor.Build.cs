namespace UnrealBuildTool.Rules
{
	public class NvFlowEditor : ModuleRules
	{
		public NvFlowEditor(TargetInfo Target)
		{
			PrivateIncludePaths.AddRange(
				new string[] {
					"NvFlowEditor/Private",
                    "../Include/include",
					// ... add other private include paths required here ...
				}
				);

            PublicIncludePaths.AddRange(
                new string[] { 
                    "NvFlowEditor/Public"
                }
            );

			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"NvFlow",
				    "Core",
				    "CoreUObject",
				    "AssetTools",
    				"Engine",
                    "UnrealEd",
					"RenderCore",
                    "Renderer",
				    "PropertyEditor",
                    "XmlParser"

				}
				);

            
		    PrivateIncludePathModuleNames.AddRange(
			    new string[] {
                    "NvFlow",
// 				    "AssetTools",
// 				    "AssetRegistry"
			    });
/*

		    DynamicallyLoadedModuleNames.AddRange(
			    new string[] {
				    "AssetTools",
				    "AssetRegistry"
			    });
*/

		}
	}
}