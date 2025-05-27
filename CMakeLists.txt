cmake_minimum_required(VERSION 3.0)

# Setting build options and C++ language standard:
set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel.")
set(CMAKE_CXX_STANDARD 17)
# add_compile_options(-w)

project(${PROJECT_NAME} CXX) # Specifies the target project
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Export compile_commands.json

# Project Variables:
set(PROJECT_NAME "PRISMA")
set(THIRDPARTY_DIR "deps")

file(GLOB_RECURSE SHADERS
  shaders/*
  )

file(GLOB_RECURSE MODELS
  res/models/*
  )

file(GLOB_RECURSE FONTS
  res/fonts/*
  )

file(GLOB_RECURSE TEXTURES
  res/textures/*
  )

file(GLOB_RECURSE SCENES
  res/scenes/*
  )

file(GLOB_RECURSE FONTS
  fonts/*
  )

# Project and Source Groups:
add_executable(${PROJECT_NAME} # Creates an executable file with a name that matches PROJECT_NAME
  # Build the executable from these files
  src/main.cpp 

  src/camera/camera.hpp
  src/camera/camera.cpp

  src/mesh/mesh.hpp
  src/mesh/mesh.cpp

  src/mesh/mesh_utils.hpp
  src/mesh/mesh_utils.cpp

  src/shader/shader.hpp
  src/shader/shader.cpp

  src/texture/texture.hpp
  src/texture/texture.cpp

  src/font/font.hpp
  src/font/font.cpp

  src/scene_node/scene_node.hpp
  src/scene_node/scene_node.cpp

  src/player/player.hpp
  src/player/player.cpp

  src/orb/orb.hpp
  src/orb/orb.cpp

  src/lamp/Lamp.h
  src/lamp/Lamp.cpp

  src/utils/opengl_utils.h
  src/utils/opengl_utils.cpp

  ${SHADERS}

  ${MODELS}

  ${FONTS}

  ${TEXTURES}

  ${SCENES}
)

## Source groups represent folders in the Visual Studio Solution Explorer
source_group("Code"
  FILES
  src/main.cpp
  )

source_group("Code\\Mesh"
  FILES
  src/mesh/mesh.cpp
  src/mesh/mesh.hpp
  )

source_group("Code\\Mesh\\Mesh Utilities"
  FILES
  src/mesh/mesh_utils.hpp
  src/mesh/mesh_utils.cpp
  )

source_group("Code\\Shader"
  FILES
  src/shader/shader.hpp
  src/shader/shader.cpp
  )

source_group("Code\\Texture"
  FILES
  src/texture/texture.hpp
  src/texture/texture.cpp
  )

source_group("Code\\Font"
  FILES
  src/font/font.hpp
  src/font/font.cpp
  )

source_group("Code\\Camera"
  FILES
  src/camera/camera.hpp
  src/camera/camera.cpp
  )

source_group("Code\\Scene Graph"
  FILES
  src/scene_node/scene_node.hpp
  src/scene_node/scene_node.cpp
  )

source_group("Code\\Different Scene Objects"
  FILES
  src/player/player.hpp
  src/player/player.cpp

  src/orb/orb.hpp
  src/orb/orb.cpp
  )

source_group("Shaders"
  FILES
  ${SHADERS}
  )

source_group("Resources\\Models"
  FILES
  ${MODELS}
  )

 source_group("Resources\\Fonts"
  FILES
  ${FONTS}
  )

 source_group("Resources\\Textures"
  FILES
  ${TEXTURES}
  )

 source_group("Resources\\Scenes"
  FILES
  ${SCENES}
  )

# Linking to third party libraries:
## OpenGL:
find_package(OpenGL REQUIRED)
target_link_libraries(${PROJECT_NAME} OpenGL::GL)

## GLAD:
add_subdirectory(${THIRDPARTY_DIR}/glad)
target_link_libraries(${PROJECT_NAME} glad)

## GLFW:
option(GLFW_BUILD_EXAMPLES OFF) # Disabling additional build targets
option(GLFW_BUILD_TESTS    OFF)
option(GLFW_BUILD_DOCS     OFF)
option(GLFW_INSTALL        OFF)
option(GLFW_VULKAN_STATIC  OFF)

add_subdirectory(${THIRDPARTY_DIR}/glfw)
target_include_directories(${PROJECT_NAME} PRIVATE ${THIRDPARTY_DIR}/glfw/include)
target_link_libraries(${PROJECT_NAME} glfw)

## GLM:
option(GLM_TEST_ENABLE OFF) # Disabling additional build targets
option(GLM_BUILD_INSTALL OFF)

add_subdirectory(${THIRDPARTY_DIR}/glm)
target_include_directories(${PROJECT_NAME} PRIVATE ${THIRDPARTY_DIR}/glm)
target_link_libraries(${PROJECT_NAME} glm)

## STB_IMAGE:
## STB_TRUETYPE:
target_include_directories(${PROJECT_NAME} PRIVATE ${THIRDPARTY_DIR}/nothings)

## TINY_OBJ_LOADER:
target_include_directories(${PROJECT_NAME} PRIVATE ${THIRDPARTY_DIR}/syoyo)

## JSON:
target_include_directories(${PROJECT_NAME} PRIVATE ${THIRDPARTY_DIR}/nlohmann)

## ASSIMP:

option(${PROJECT_NAME}_ASSIMP_EXPORT_OBJECTS "Enable exporting our models in optimized assbin format." OFF)

if (${PROJECT_NAME}_ASSIMP_EXPORT_OBJECTS)
  message(STATUS "[${PROJECT_NAME}] Enabled exporting our models in optimized assbin format.")

  target_compile_definitions(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_ASSIMP_EXPORT_OBJECTS)

  set(ASSIMP_BUILD_OBJ_IMPORTER ON) # Build OBJ and FBX importers ONLY
  set(ASSIMP_BUILD_FBX_IMPORTER ON)

  set(ASSIMP_BUILD_ASSBIN_EXPORTER ON) # Enable assbin exporter
  set(ASSIMP_NO_EXPORT FALSE CACHE BOOL "Disable export API." FORCE) # Enable export API
else()
  set(ASSIMP_NO_EXPORT TRUE  CACHE BOOL "Disable export API." FORCE) # Disable export API

  # Disable all assimp post-processing steps, since we should have already done them while exporting optimized assbin files.
  set(ASSIMP_BUILD_NO_ARMATUREPOPULATE_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_CALCTANGENTS_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_DEBONE_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_DROPFACENORMALS_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_EMBEDTEXTURES_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_FINDDEGENERATES_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_FINDINSTANCES_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_FINDINVALIDDATA_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_FIXINFACINGNORMALS_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_FLIPUVS_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_FLIPWINDINGORDER_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_GENBOUNDINGBOXES_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_GENFACENORMALS_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_GENUVCOORDS_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_GENVERTEXNORMALS_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_GLOBALSCALE_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_IMPROVECACHELOCALITY_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_JOINVERTICES_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_LIMITBONEWEIGHTS_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_MAKELEFTHANDED_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_OPTIMIZEGRAPH_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_OPTIMIZEMESHES_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_PRETRANSFORMVERTICES_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_REMOVE_REDUNDANTMATERIALS_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_REMOVEVC_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_SORTBYPTYPE_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_SPLITBYBONECOUNT_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_SPLITLARGEMESHES_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_TRANSFORMTEXCOORDS_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_TRIANGULATE_PROCESS TRUE)
  set(ASSIMP_BUILD_NO_VALIDATEDS_PROCESS TRUE)
endif()

set(ASSIMP_BUILD_ASSBIN_IMPORTER ON) # Build ASSBIN importer ONLY
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT CACHE BOOL OFF)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT CACHE BOOL OFF)
set(ASSIMP_BUILD_TESTS                    CACHE BOOL OFF)
set(ASSIMP_BUILD_ASSIMP_TOOLS             CACHE BOOL OFF)

add_subdirectory(${THIRDPARTY_DIR}/assimp)
target_include_directories(${PROJECT_NAME} PRIVATE ${THIRDPARTY_DIR}/assimp/include)
target_link_libraries(${PROJECT_NAME} assimp)
target_compile_options(assimp PRIVATE -Wno-unknown-pragmas)

# Options:
set(BUILD_SHARED_LIBS FALSE CACHE BOOL "Enforce static linking." FORCE) # Build static libraries
set(BUILD_STATIC_LIBS TRUE  CACHE BOOL "Enforce static linking." FORCE)

# Visual Studio Convenience:
## Set as the startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

# Adding compile definitions:
target_compile_definitions(${PROJECT_NAME} PRIVATE
                           UNICODE
                           _UNICODE
                           _CRT_SECURE_NO_WARNINGS)


if (CMAKE_BUILD_TYPE MATCHES Debug)
  target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG)
  
  if (MSVC)
    set(SANITIZERS_OPTIONS /fsanitize=address /RTC1)
    set(COMPILE_FLAGS /Zi /Od /Ob0 /Od ${SANITIZERS_OPTIONS})
    set(LINK_FLAGS ${SANITIZERS_OPTIONS})
  elseif (NOT MINGW)
    set(SANITIZERS_OPTIONS -fsanitize=address -fsanitize=leak -fsanitize=undefined)
    set(COMPILE_FLAGS ${SANITIZERS_OPTIONS} -g3)
    set(LINK_FLAGS ${SANITIZERS_OPTIONS} -fno-omit-frame-pointer -fno-optimize-sibling-calls)
  endif()

  target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_FLAGS})

  target_link_options(${PROJECT_NAME} PRIVATE ${LINK_FLAGS})
endif()
