// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class VectorizingAnimation : ModuleRules
{
	public VectorizingAnimation(TargetInfo Target)
	{
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UnrealEd" });

        PrivateDependencyModuleNames.AddRange(new string[] { "RHI", "RenderCore", "ShaderCore", "DesktopPlatform" });

        Definitions.AddRange(new string[] { "_USE_MATH_DEFINES", "USE_CGAL" });

        // zlib must linked first and other must after opencv linked
        AddThirdPartyPrivateStaticDependencies(Target, "zlib", "ffmpeg", "OpenCV");
        // for opencv need
        AddThirdPartyPrivateStaticDependencies(Target,
                "libJasper",
                "libJPG",
                "UElibPNG",
                "libTiff",
                "UEOpenEXR",
                "cuda",
                "libiconv"
                );
        // for openmesh
        bUseRTTI = true;
        AddThirdPartyPrivateStaticDependencies(Target, "OpenMesh");

        AddThirdPartyPrivateStaticDependencies(Target, "boost", "MPFR", "MPIR", "Eigen3", "OpenBLAS", "fftw", "CGAL");

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
