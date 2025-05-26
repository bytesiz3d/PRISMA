cmake_minimum_required(VERSION 3.0)

# Setting build options and C++ language standard:
set(CMAKE_BUILD_TYPE Debug)
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
set(ASSIMP_BUILD_OBJ_IMPORTER ON) # Build OBJ and FBX importers ONLY
set(ASSIMP_BUILD_FBX_IMPORTER ON)

set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT CACHE BOOL OFF)
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT CACHE BOOL OFF)
set(ASSIMP_BUILD_TESTS                    CACHE BOOL OFF)
set(ASSIMP_BUILD_ASSIMP_TOOLS             CACHE BOOL OFF)
set(ASSIMP_NO_EXPORT  TRUE  CACHE BOOL "Disable export API."     FORCE)

add_subdirectory(${THIRDPARTY_DIR}/assimp)
target_include_directories(${PROJECT_NAME} PRIVATE ${THIRDPARTY_DIR}/assimp/include)
target_link_libraries(${PROJECT_NAME} assimp)

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
endif()
