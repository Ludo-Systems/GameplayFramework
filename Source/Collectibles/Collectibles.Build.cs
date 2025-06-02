using UnrealBuildTool;

public class Collectibles : ModuleRules
{
    public Collectibles(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "Slate",
                "SlateCore",
                "GameplayFramework",
                "GameplayAbilities",
                "GameplayTags",
                "ModelViewViewModel"
            }
        );
    }
}