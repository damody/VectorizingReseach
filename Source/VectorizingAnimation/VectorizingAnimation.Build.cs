// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class VectorizingAnimation : ModuleRules
{
	public VectorizingAnimation(TargetInfo Target)
	{
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
        if (UEBuildConfiguration.bBuildEditor)
        {
            PublicDependencyModuleNames.AddRange(new string[] { "UnrealEd" });
        }

        PrivateDependencyModuleNames.AddRange(new string[] { "RHI", "RenderCore", "ShaderCore", "DesktopPlatform" });

        Definitions.AddRange(new string[] { "_USE_MATH_DEFINES", "USE_CGAL", "_CRT_SECURE_NO_WARNINGS" });

        // zlib must linked first and other must after opencv linked
        AddThirdPartyPrivateStaticDependencies(Target, "zlib", "ffmpeg", "OpenCV2");
        // for opencv need
        AddThirdPartyPrivateStaticDependencies(Target,
                "cuda65",
                "libiconv"
                );
        // for openmesh
        bUseRTTI = true;
        AddThirdPartyPrivateStaticDependencies(Target, "OpenMesh");

        AddThirdPartyPrivateStaticDependencies(Target, "Eigen3", "boost", "MPFR", "MPIR", "OpenBLAS", "fftw", "CGAL");

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");
		// if ((Target.Platform == UnrealTargetPlatform.Win32) || (Target.Platform == UnrealTargetPlatform.Win64))
		// {
		//		if (UEBuildConfiguration.bCompileSteamOSS == true)
		//		{
		//			DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");
		//		}
		// }
	}
}
