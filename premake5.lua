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
		"%{prj.name}/include"
	}

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
		symbols "On"
	
	filter "configurations:Release"
		defines "EMT_RELEASE"
		optimize "On"
	
	filter "configurations:Dist"
		defines "EMT_DIST"
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
		symbols "On"
	
	filter "configurations:Release"
		defines "EMT_RELEASE"
		optimize "On"
	
	filter "configurations:Dist"
		defines "EMT_DIST"
		optimize "On"