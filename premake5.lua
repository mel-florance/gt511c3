workspace "gt511c3"
	architecture "x64"
	startproject "Scanner"

	configurations {
		"Debug",
		"Release"
	}

IncludeDir = {}
IncludeDir["GLFW"] = "gt511c3/vendor/GLFW/include"
IncludeDir["Glad"] = "gt511c3/vendor/Glad/include"
IncludeDir["imgui"] = "gt511c3/vendor/imgui"
IncludeDir["stb"] = "gt511c3/vendor/stb"
IncludeDir["serial"] = "gt511c3/vendor/serial/include"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "gt511c3/vendor/GLFW"
include "gt511c3/vendor/Glad"	
include "gt511c3/vendor/imgui"
include "gt511c3/vendor/serial"

project "gt511c3"
	location "gt511c3"
	kind "StaticLib"
	language "C++"
 
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	linkoptions {
		"-IGNORE:4221",
		"-IGNORE:4006"
	}

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs {
		"%{IncludeDir.serial}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.stb}",
		"%{prj.name}/src"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		postbuildcommands {
			"{COPY} ../Scanner/data ../bin/Release-windows-x86_64/Scanner/data",
			"{COPY} ../Scanner/data ../bin/Debug-windows-x86_64/Scanner/data",
		}

		defines {
			"IMGUI_API=__declspec(dllexport)",
			"GT_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE",
			"_CRT_SECURE_NO_WARNINGS"
		}

		links {
			"serial",
			"GLFW",
			"Glad",
			"imgui",
			"Setupapi.lib",
			"opengl32.lib",
		}

	filter "system:linux"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		postbuildcommands {
			"{COPY} ../Scanner/data ../bin/Release-linux-x86_64/Scanner/data",
			"{COPY} ../Scanner/data ../bin/Debug-linux-x86_64/Scanner/data",
		}

		defines {
			"IMGUI_API=__attribute__((visibility(\"default\")))",
			"GT_PLATFORM_LINUX",
			"GLFW_INCLUDE_NONE",
			"_CRT_SECURE_NO_WARNINGS"
		}

		links {
			"serial",
			"GLFW",
			"Glad",
			"imgui",
			"X11",
			"XCursor",
			"Xrandr",
			"Xinerama",
			"GL",
			"dl",
			"pthread",
			"Setupapi.lib",
			"opengl32.lib"
		}

	filter "configurations:Debug"
		defines "GT_DEBUG"
		symbols "On"
		
	filter "configurations:Release"
		defines "GT_RELEASE"
		runtime "Release"
		optimize "On"

project "Scanner"
	location "Scanner"
	language "C++"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files {
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/serial/include/**",
		"%{prj.name}/vendor/serial/src/**"
	}

	includedirs {
		"%{IncludeDir.serial}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"gt511c3/src",
		"gt511c3/vendor",
		"Scanner/src"
	}

	links {
		"serial",
		"gt511c3"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"GT_PLATFORM_WINDOWS"
		}

	filter "system:linux"
		cppdialect "C++17"
		staticruntime "On"
		systemversion "latest"

		defines {
			"GT_PLATFORM_LINUX"
		}

	filter "configurations:Debug"
		defines "GT_DEBUG"
		kind "ConsoleApp"
		symbols "On"
		
	filter "configurations:Release"
		defines "GT_RELEASE"
		optimize "On"
		symbols "Off"
		kind "WindowedApp"
		entrypoint "mainCRTStartup" 
