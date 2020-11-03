workspace "gt511c3"
	architecture "x64"
	startproject "Scanner"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "gt511c3/vendor/GLFW/include"
IncludeDir["Glad"] = "gt511c3/vendor/Glad/include"
IncludeDir["ImGui"] = "gt511c3/vendor/imgui"
IncludeDir["stb"] = "gt511c3/vendor/stb"
IncludeDir["serial"] = "gt511c3/vendor/serial"

include "gt511c3/vendor/GLFW"
include "gt511c3/vendor/Glad"	
include "gt511c3/vendor/ImGui"
include "gt511c3/vendor/serial"

project "gt511c3"
	location "gt511c3"
	kind "StaticLib"
	staticruntime "on"
	language "C++"
 
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	linkoptions { "-IGNORE:4221", "-IGNORE:4006" }

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.stb}"
		"%{IncludeDir.serial}"
	}

	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
		"serial"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"IMGUI_API=__declspec(dllexport)",
			"GT_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE",
			"_CRT_SECURE_NO_WARNINGS"
		}

		postbuildcommands
		{
			--"{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox",
			--"{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Server",
		}

	filter "configurations:Debug"
		defines "GT_DEBUG"
		buildoptions {"/MTd"}
		symbols "On"
		
	filter "configurations:Release"
		defines "GT_RELEASE"
		runtime "Release"
		optimize "On"
		
	filter "configurations:Dist"
		defines "GT_DIST"
		runtime "Release"
		optimize "On"


project "Scanner"
	location "Scanner"

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
		"Scanner/src"
	}

	links
	{
		"gt511c3"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"GT_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "GT_DEBUG"
		kind "ConsoleApp"
		symbols "On"
		
	filter "configurations:Release"
		defines "GT_RELEASE"
		optimize "On"
		kind "WindowedApp"
		entrypoint "mainCRTStartup" 
		
	filter "configurations:Dist"
		defines "GT_DIST"
		optimize "On"
		kind "WindowedApp"
		entrypoint "mainCRTStartup" 
