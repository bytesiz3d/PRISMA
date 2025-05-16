#include <fstream>
#include <iostream>
#include <vector>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include "camera/camera.hpp"
#include "font/font.hpp"
#include "mesh/mesh_utils.hpp"
#include "orb/orb.hpp"
#include "player/player.hpp"
#include "scene_node/scene_node.hpp"
#include "shader/shader.hpp"
#include "texture/texture.hpp"

// *NOT SEPARATELY BUILT*
// TODO: Add shaderIDs to the JSON file
#include "scene/scene.hpp"

// Window dimensions
GLuint WIDTH = 1280, HEIGHT = 720;
int level;

// Start our window
bool InitWindow();

void RunGame();

// ====================================================================================================
int main() {
  std::cout << "Enter level (1 or 2): ";
  std::cin >> level;
  if (level != 1 && level != 2) {
    std::cout << "Invalid level. Exiting." << std::endl;
    return -1;
  }
  if (!InitWindow())
    return -1;
  RunGame();
  glfwTerminate();
  return 0;
}

// ====================================================================================================
void RunGame() {
  // Compile and link the shader program
  // GLuint cubeShaderProgram = Shader::LoadShader("../shaders/texture.vert", "../shaders/texture.frag");
  GLuint cubeShaderProgram = Shader::LoadShader("../shaders/light.vert", "../shaders/light.frag");
  GLuint hudShaderProgram = Shader::LoadShader("../shaders/hud.vert", "../shaders/color.frag");

  glUseProgram(cubeShaderProgram);
  Scene::VP_location = glGetUniformLocation(cubeShaderProgram, "VP");
  Scene::texture_sampler_location = glGetUniformLocation(cubeShaderProgram, "texture_sampler");
  Scene::cam_pos_location = glGetUniformLocation(cubeShaderProgram, "cam_position");

  // Create meshes
  Mesh* cube = Mesh_Utils::WhiteCube();
  Mesh* sphere = Mesh_Utils::Sphere();
  // Mesh* player = Mesh_Utils::OBJMesh("../res/models/player");
  Mesh* player = Mesh_Utils::FBXMesh("../res/models/player");
  Mesh* lamp = Mesh_Utils::FBXMesh("../res/models/lamp");
  Scene::meshes[MESH_NULL] = nullptr;
  Scene::meshes[MESH_CUBE] = cube;
  Scene::meshes[MESH_SPHERE] = sphere;
  Scene::meshes[MESH_MODEL0] = player;
  Scene::meshes[MESH_MODEL4] = lamp;

  // Create textures
  Texture grid("../res/textures/tiles-256.png", GL_RGB);
  Texture wood("../res/textures/albedo.jpg", GL_RGB);
  Texture exit("../res/textures/exit.png", GL_RGB);
  Texture wall("../res/textures/wall.jpg", GL_RGB);
  Texture white;
  Scene::textures[MESH_TEXTURE_NULL] = &white;
  Scene::textures[MESH_TEXTURE0] = &wood;
  Scene::textures[MESH_TEXTURE1] = &grid;
  Scene::textures[MESH_TEXTURE2] = &exit;
  Scene::textures[MESH_TEXTURE3] = &wall;

  // Initialize scene
  if (level == 1)
    Scene::InitScene("../res/scenes/level1.json");
  else if (level == 2)
    Scene::InitScene("../res/scenes/level2.json");

  Scene::UploadLights(cubeShaderProgram);

  // Create the camera object
  Scene::camera.aspectRatio = (float)WIDTH / HEIGHT;

  glUniform1f(Scene::texture_sampler_location, 0);
  // Game loop
  while (!glfwWindowShouldClose(Scene::window)) {
    // Check if any events have been activated (key pressed, mouse moved etc.)
    // and call corresponding response functions
    glfwPollEvents();

    Scene::UpdateData();

    // Render
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(cubeShaderProgram);

    // Pass VP, camera position
    glm::mat4 VP = Scene::camera.ViewProjectionMatrix();
    glUniformMatrix4fv(Scene::VP_location, 1, false, glm::value_ptr(VP));
    glUniform3f(Scene::cam_pos_location, Scene::camera.position.x, Scene::camera.position.y, Scene::camera.position.z);

    Scene::DrawScene(Scene::root, cubeShaderProgram);
    Scene::DrawScene(Scene::player, cubeShaderProgram);
    Scene::DrawScene(Scene::lamp, cubeShaderProgram);
    Scene::DrawScene(Scene::hud, hudShaderProgram);

    // Swap the screen buffers
    glfwSwapBuffers(Scene::window);
  }

  delete cube;
  delete player;
  delete sphere;
  delete lamp;
  
  glfwTerminate();
  glDeleteProgram(cubeShaderProgram);
  glDeleteProgram(hudShaderProgram);

  Scene::DeleteAllPointers();
}

// ====================================================================================================
bool InitWindow() {
  std::cout << "Starting GLFW context, OpenGL 4.4" << std::endl;

  // Init GLFW
  glfwInit();

  // get the screen resolution
  const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
  WIDTH = mode->width;
  HEIGHT = mode->height;

  // Set all the required options for GLFW
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_DEPTH_BITS, 32);
  glfwWindowHint(GLFW_SAMPLES, 16);

  // Create a GLFWwindow object that we can use for GLFW's functions
  Scene::window = glfwCreateWindow(WIDTH, HEIGHT, "GFX Playground", glfwGetPrimaryMonitor(), NULL);
  glfwMakeContextCurrent(Scene::window);
  if (Scene::window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return false;
  }

  // Set the required callback functions
  glfwSetKeyCallback(Scene::window, Scene::KeyCallback);
  glfwSetMouseButtonCallback(Scene::window, Scene::MouseCallback);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize OpenGL context" << std::endl;
    return false;
  }

  // Define the viewport dimensions
  glViewport(0, 0, WIDTH, HEIGHT);

  // Black background
  glClearColor(0, 0, 0, 1);

  // Set the depth test function
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);

  // Anti-aliasing
  glEnable(GL_LINE_SMOOTH);

  // Enable blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glBlendEquation(GL_FUNC_ADD);

  // Back-face culling
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);

  return true;
}

#include "scene/scene.cpp"
