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
IncludeDir["Glad"] = "EMT/include/Glad/include"
IncludeDir["ImGui"] = "EMT/include/imgui"
IncludeDir["StbImage"] = "EMT/include/stb_image"
IncludeDir["Assimp"] = "EMT/include/assimp"

include "EMT/include/GLFW"
include "EMT/include/Glad"
include "EMT/include/imgui"

project "EMT"
	location "EMT"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	files 
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/include/stb_image/**.h",
		"%{prj.name}/include/stb_image/**.cpp"
	}

	defines 
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.StbImage}",
		"%{IncludeDir.Assimp}"
	}


	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib",
	}

	pchheader "emtpch.h"
	pchsource "EMT/src/emtpch.cpp"

	filter "system:windows"
		
		systemversion "10.0"
		
		defines
		{
			"EMT_PLATFORM_WINDOWS",
			"EMT_BUILD_DLL"
		}

	filter "configurations:Debug"
		defines "EMT_DEBUG"
		defines "EMT_ENABLE_ASSERTS"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		defines "EMT_RELEASE"
		runtime "Release"
		optimize "on"
	
	filter "configurations:Dist"
		defines "EMT_DIST"
		runtime "Release"
		optimize "on"


project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"

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

	libdirs
	{ 
		"%{prj.name}/lib" 
	}

	links 
	{
		"EMT",
		"assimp-vc143-mtd.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		
		defines
		{
			"EMT_PLATFORM_WINDOWS",
		}

	filter "configurations:Debug"
		defines "EMT_DEBUG"
		runtime "Debug"
		symbols "on"
	
	filter "configurations:Release"
		defines "EMT_RELEASE"
		runtime "Release"
		optimize "on"
	
	filter "configurations:Dist"
		defines "EMT_DIST"
		runtime "Release"
		optimize "on"