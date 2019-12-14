cmake_minimum_required(VERSION 3.0)

# Setting build options and C++ language standard:
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)
add_compile_options(-w)

# Project Variables:
set(PROJECT_NAME "PRISMA")
set(THIRDPARTY_DIR "deps")

# Project and Source Groups:
project(${PROJECT_NAME} CXX) # Specifies the target project
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

  src/scene_node/scene_node.hpp
  src/scene_node/scene_node.cpp

  src/player/player.hpp
  src/player/player.cpp
  )

## Source groups represent folders in the Visual Studio Solution Explorer
source_group("Mesh"
  FILES
  src/mesh/mesh.cpp
  src/mesh/mesh.hpp
  )

source_group("Mesh\\Mesh Utilities"
  FILES
  src/mesh/mesh_utils.hpp
  src/mesh/mesh_utils.cpp
  )

source_group("Shader"
  FILES
  src/shader/shader.hpp
  src/shader/shader.cpp
  )

source_group("Texture"
  FILES
  src/texture/texture.hpp
  src/texture/texture.cpp
  )

source_group("Camera"
  FILES
  src/camera/camera.hpp
  src/camera/camera.cpp
  )

source_group("Scene Graph"
  FILES
  src/scene_node/scene_node.hpp
  src/scene_node/scene_node.cpp
  )

source_group("Player Controller"
  FILES
  src/player/player.hpp
  src/player/player.cpp
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
option(GLFW_BUILD_DOCS 	 OFF)
option(GLFW_INSTALL	 OFF)
option(GLFW_VULKAN_STATIC  OFF)

add_subdirectory(${THIRDPARTY_DIR}/glfw)
target_include_directories(${PROJECT_NAME} PRIVATE ${THIRDPARTY_DIR}/glfw/include)
target_link_libraries(${PROJECT_NAME} glfw)

## GLM:
option(GLM_TEST_ENABLE OFF) # Disabling additional build targets

add_subdirectory(${THIRDPARTY_DIR}/glm)
target_include_directories(${PROJECT_NAME} PRIVATE ${THIRDPARTY_DIR}/glm)
target_link_libraries(${PROJECT_NAME} glm_static)

## STB_IMAGE:
target_include_directories(${PROJECT_NAME} PRIVATE ${THIRDPARTY_DIR}/stb)

## TINY_OBJ_LOADER:
target_include_directories(${PROJECT_NAME} PRIVATE ${THIRDPARTY_DIR}/syoyo)

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
