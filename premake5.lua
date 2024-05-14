workspace "EMT"
	architecture "x64"
	startproject "Sandbox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "EMT/include/GLFW/include"

include "EMT/include/GLFW"

project "EMT"
	location "EMT"
	kind "SharedLib"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/include",
		"%{IncludeDir.GLFW}"
	}

	links
	{
		"GLFW",
		"opengl32.lib"
	}

	pchheader "emtpch.h"
	pchsource "EMT/src/emtpch.cpp"

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "10.0"
		
		defines
		{
			"EMT_PLATFORM_WINDOWS",
			"EMT_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox/\"")
		}

	filter "configurations:Debug"
		defines "EMT_DEBUG"
		defines "EMT_ENABLE_ASSERTS"
		buildoptions "/MDd"
		symbols "On"
	
	filter "configurations:Release"
		defines "EMT_RELEASE"
		buildoptions "/MD"
		optimize "On"
	
	filter "configurations:Dist"
		defines "EMT_DIST"
		buildoptions "/MD"
		optimize "On"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
	}

	includedirs
	{
		"EMT/include",
		"EMT/src"
	}

	links 
	{
		"EMT"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "10.0"
		
		defines
		{
			"EMT_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "EMT_DEBUG"
		buildoptions "/MDd"
		symbols "On"
	
	filter "configurations:Release"
		defines "EMT_RELEASE"
		buildoptions "/MD"
		optimize "On"
	
	filter "configurations:Dist"
		defines "EMT_DIST"
		buildoptions "/MD"
		optimize "On"