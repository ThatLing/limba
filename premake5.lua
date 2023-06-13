assert(_ACTION ~= nil, "no action (vs20**, gmake or xcode for example) provided!")

-- Clean Function --
newaction {
   trigger     = "clean",
   description = "clean the software",
   execute     = function()
      print("Cleaning project")
      os.rmdir("./limbo")
   end
}

if (_ACTION ~= "vs2022" and _ACTION ~= "clean") then
	error("The only supported compilation platform for this project (limbo) on Windows is Visual Studio 2022.")
end


workspace "limbo"
	language "C++"
	cppdialect "C++20"
	systemversion "latest"
	toolset "clang"

	kind "ConsoleApp"
	targetextension ".exe"

	location "limbo"
	objdir "limbo/intermediate"

	flags { "NoPCH", "MultiProcessorCompile", "NoManifest" }
	defines {
		"_CRT_NONSTDC_NO_WARNINGS",
		"_CRT_SECURE_NO_WARNINGS",
		"STRICT"
	}
	vectorextensions "AVX2"
	configurations { "Release", "Debug" }
	architecture "x86_64"
	exceptionhandling "SEH"

	filter "configurations:Release"
		symbols "Off"
		optimize "Speed"
		floatingpoint "Fast"
		omitframepointer "On"
		flags { "LinkTimeOptimization" }

		defines "NDEBUG"
		targetdir "limbo/release"

	filter "configurations:Debug"
		symbols "Full"
		optimize "Debug"

		defines { "DEBUG", "_DEBUG" }
		targetdir "limbo/debug"

	filter {}

	project "limbo"
		files { "source/**.h", "source/**.hpp", "source/**.c", "source/**.cpp" }
		vpaths {
			["Header Files/*"] = { "source/**.h", "source/**.hpp" },
			["Source Files/*"] = { "source/**.c", "source/**.cpp" }
		}

		includedirs {
			"source",
			"source/limbo",
		}
