 --
-- premake_config.lua
--
-- Premake solution to build the C++ port of the EDK STD projects.
-- Ivan Sancho, Escuela Superior de Arte y Tecnologia (ESAT) Valencia, 2023/24
-- Algorithms & Data Structures
--
-- All rights are reserved.
--

PROJ_DIR = path.getabsolute("./..")

PROJECT_NAMES = {
  "PR00_TestMemoryManager",
  "PR01_MemoryNode",
  "PR02_Vector",
  "PR05_List",
  "PR06_DLList",
  "PR09_Stack",
  "PR10_Queue",
}

-- Solution workspace declaration:
workspace("DS_ALG_AI1_CPP" .. _ACTION)
  location(PROJ_DIR .. "/build/")
  language "C++"
  cppdialect "C++17"
  kind "ConsoleApp"
  startproject "PR01_MemoryNode"
  platforms {
    "x32",
    "x64",
    "Native",
  }
  defines {
    "_CRT_SECURE_NO_WARNINGS",
  }
  configurations { "Debug", "Release" }

-- Workspace "Debug" configuration:
filter { "configurations:Debug" }
  defines {
    "DEBUG",
  }
  symbols "On"
  warnings "Extra"
  targetsuffix "_d"

-- Workspace "Release" configuration:
filter { "configurations:Release" }
  defines {
    "NDEBUG",
  }
  symbols "Off"
  warnings "Extra"
  optimize "Size"
  flags {
    "No64BitChecks",
    "NoPCH",
  }


--[[********************* PROJECT GENERATOR FUNCTION *********************]]--
function GenerateProject(project_name, project_group)
  project(project_name)
    location (PROJ_DIR .. "/build/" .. project_name .. "/" .. _ACTION)
    includedirs {
      path.join(PROJ_DIR, "include/"),
      path.join(PROJ_DIR, "deps/include/"),
    }
    files {
      path.join(PROJ_DIR, "include/*.h"),
      path.join(PROJ_DIR, "include/common_def.h"),
      path.join(PROJ_DIR, "deps/include/EDK_MemoryManager/*.h"),
    }

    libdirs { path.join(PROJ_DIR, "deps/lib/EDK_MemoryManager") }

    --Debug filters:
    filter { "configurations:Debug", "platforms:x32" }
      architecture "x86"
      defines { "x32" }
      targetdir (path.join(PROJ_DIR, "bin/Debug/x32"))
      objdir(path.join(PROJ_DIR, "build/" .. project_name .. "/Debug"))
      targetsuffix "_d"
      links { path.join(PROJ_DIR, "deps/lib/EDK_MemoryManager/x32/EDK_MemoryManager_Windows_d") }
    filter { "configurations:Debug", "platforms:x64" }
      architecture "x86_64"
      defines { "x64" }
      targetdir (path.join(PROJ_DIR, "bin/Debug/x64"))
      objdir(path.join(PROJ_DIR, "build/" .. project_name .. "/Debug"))
      targetsuffix "_d"
      links { path.join(PROJ_DIR, "deps/lib/EDK_MemoryManager/x64/EDK_MemoryManager_Windows_d") }
    --Release filters:
    filter { "configurations:Release", "platforms:x32" }
      architecture "x86"
      defines { "x32" }
      targetdir (path.join(PROJ_DIR, "bin/Release/x32/"))
      objdir(path.join(PROJ_DIR, "build/" .. project_name .. "/Release"))
      links { path.join(PROJ_DIR, "deps/lib/EDK_MemoryManager/x32/EDK_MemoryManager_Windows") }
    filter { "configurations:Release", "platforms:x64" }
      architecture "x86_64"
      defines { "x64" }
      targetdir (path.join(PROJ_DIR, "bin/Release/x64/"))
      objdir(path.join(PROJ_DIR, "build/" .. project_name .. "/Release"))
      links { path.join(PROJ_DIR, "deps/lib/EDK_MemoryManager/x64/EDK_MemoryManager_Windows") }
end --Function "GenerateProject" ends here.

--Calling the Lua function:
for i, prj in ipairs(PROJECT_NAMES) do
  GenerateProject(prj)
end


project "PR00_TestMemoryManager"
  files {
    path.join(PROJ_DIR, "tests/test_memory_manager.cc"),
  }

project "PR01_MemoryNode"
  files {
    path.join(PROJ_DIR, "include/adt_memory_node.h"),
    path.join(PROJ_DIR, "src/adt_memory_node.cc"),
    path.join(PROJ_DIR, "tests/test_memory_node.cc"),
  }

project "PR02_Vector"
  files {
    path.join(PROJ_DIR, "include/adt_memory_node.h"),
    path.join(PROJ_DIR, "src/adt_memory_node.cc"),
    path.join(PROJ_DIR, "include/adt_vector.h"),
    path.join(PROJ_DIR, "src/adt_vector.cc"),
    path.join(PROJ_DIR, "tests/test_vector.cc"),
  }

project "PR05_List"
  files {
    path.join(PROJ_DIR, "include/adt_memory_node.h"),
    path.join(PROJ_DIR, "src/adt_memory_node.cc"),
    path.join(PROJ_DIR, "include/adt_list.h"),
    path.join(PROJ_DIR, "src/adt_list.cc"),
    path.join(PROJ_DIR, "tests/test_list.cc"),
  }

project "PR06_DLList"
  files {
    path.join(PROJ_DIR, "include/adt_memory_node.h"),
    path.join(PROJ_DIR, "src/adt_memory_node.cc"),
    path.join(PROJ_DIR, "include/adt_dllist.h"),
    path.join(PROJ_DIR, "src/adt_dllist.cc"),
    path.join(PROJ_DIR, "tests/test_dllist.cc"),
  }

project "PR09_Stack"
  files {
    path.join(PROJ_DIR, "include/adt_memory_node.h"),
    path.join(PROJ_DIR, "src/adt_memory_node.cc"),
    path.join(PROJ_DIR, "include/adt_stack.h"),
    path.join(PROJ_DIR, "src/adt_stack.cc"),
    path.join(PROJ_DIR, "tests/test_stack.cc"),
  }

project "PR10_Queue"
  files {
    path.join(PROJ_DIR, "include/adt_memory_node.h"),
    path.join(PROJ_DIR, "src/adt_memory_node.cc"),
    path.join(PROJ_DIR, "include/adt_queue.h"),
    path.join(PROJ_DIR, "src/adt_queue.cc"),
    path.join(PROJ_DIR, "tests/test_queue.cc"),
  }
