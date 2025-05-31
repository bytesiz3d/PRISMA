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
#include "utils/opengl_utils.h"
#include "input/input_manager.hpp"
#include <chrono>

// Window dimensions
GLuint WIDTH = 1280, HEIGHT = 720;

// Start our window
bool InitWindow();

int MainMenu();
void RunGame(int level);

// ====================================================================================================
int main() {
  if (!InitWindow())
    return -1;
  const int level = MainMenu();
  RunGame(level);
  glfwTerminate();
  return 0;
}

// ====================================================================================================
int MainMenu() {

  GLuint backgroundShader = Shader::LoadShader("../shaders/background.vert", "../shaders/background.frag");
  auto backgroundMesh = Mesh_Utils::ColoredBackground({
    glm::vec4(1, 0, 0, 1), // Red
    glm::vec4(1, 1, 0, 1), // Yellow
    glm::vec4(0, 1, 0, 1), // Green
    glm::vec4(0, 0, 1, 1), // Blue
  });
  Scene_Node background(backgroundMesh);

  // Compile and link the shader program
  GLuint shaderProgram = Shader::LoadShader("../shaders/font.vert", "../shaders/font.frag");

  // Create the font
  Font font("../fonts/airstrikebold.ttf", HEIGHT/10);

  auto [menu, levels] = Scene::InitMainMenu("../res/scenes/levels.json", font);

  glUseProgram(shaderProgram);

  // Load the font, just like a texture
  const GLint texture_sampler_location = glGetUniformLocation(shaderProgram, "texture_sampler");
  font.Bind(0);
  glUniform1i(texture_sampler_location, 0);

  int selectedLevel = 0;
  float backgroundAngle = 0.0f;

  auto lastTime = std::chrono::high_resolution_clock::now();

  // Game loop
  while (!glfwWindowShouldClose(Scene::getWindow())) {
    auto now = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float>(now - lastTime).count();
    lastTime = now;


    // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
    glfwPollEvents();

    auto [exit, startGame, level] = InputManager::ProcessMainMenuInput(levels.size());
    if (exit) {
      glfwSetWindowShouldClose(Scene::getWindow(), true);
    }

    if (startGame) {
      selectedLevel = level;
      break;  // Exit the main menu loop and start the game
    }

    for (auto & lvl : levels) {
      lvl->color = glm::vec4(1); // Reset all levels to white
    }

    levels[level-1]->color = glm::vec4(0.2f, 0.5f, 0.7f, 1);

    // Render
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgram);

    backgroundAngle += glm::radians(90.f) * deltaTime; // Rotate the background
    background.relativeModel = glm::rotate(glm::mat4(1.0f), backgroundAngle, glm::vec3(0, 0, 1));

    Scene::DrawScene(&background, backgroundShader);
    Scene::DrawScene(menu, shaderProgram);

    // Swap the screen buffers
    glfwSwapBuffers(Scene::getWindow());
  }

  glDeleteProgram(shaderProgram);
  return selectedLevel;
}

// ====================================================================================================
void RunGame(const int level) {
  // Compile and link the shader program
  // GLuint cubeShaderProgram = Shader::LoadShader("../shaders/texture.vert", "../shaders/texture.frag");
  GLuint cubeShaderProgram = Shader::LoadShader("../shaders/light.vert", "../shaders/light.frag");
  GLuint hudShaderProgram = Shader::LoadShader("../shaders/hud.vert", "../shaders/color.frag");

  glUseProgram(cubeShaderProgram);
  const GLint VP_location = glGetUniformLocation(cubeShaderProgram, "VP");
  const GLint texture_sampler_location = glGetUniformLocation(cubeShaderProgram, "texture_sampler");
  const GLint cam_pos_location = glGetUniformLocation(cubeShaderProgram, "cam_position");

  Scene scene;

  // Create meshes
  auto player = Mesh_Utils::LoadMesh("../res/models/player.assbin");
  auto lamp = Mesh_Utils::LoadMesh("../res/models/lamp.assbin");
  scene.set_meshes({
    {MESH_NULL, nullptr},
    {MESH_CUBE, Mesh_Utils::WhiteCube()},
    {MESH_SPHERE, Mesh_Utils::Sphere()},
    {MESH_MODEL0, player},
    {MESH_MODEL4, lamp}
  });

  // Create textures
  Texture grid("../res/textures/tiles-256.png", GL_RGB);
  Texture wood("../res/textures/albedo.jpg", GL_RGB);
  Texture exit("../res/textures/exit.png", GL_RGB);
  Texture wall("../res/textures/wall.jpg", GL_RGB);
  Texture white;
  scene.set_textures({
    {MESH_TEXTURE_NULL, &white},
    {MESH_TEXTURE0, &wood},
    {MESH_TEXTURE1, &grid},
    {MESH_TEXTURE2, &exit},
    {MESH_TEXTURE3, &wall}
  });

  // Initialize scene
  scene.InitScene("../res/scenes/level"+ std::to_string(level) + ".json");

  scene.UploadLights(cubeShaderProgram);

  glUniform1i(texture_sampler_location, 0);

  auto lastTime = std::chrono::high_resolution_clock::now();

  // Game loop
  while (!glfwWindowShouldClose(Scene::getWindow())) {
    auto now = std::chrono::high_resolution_clock::now();
    float deltaTime = std::chrono::duration<float>(now - lastTime).count();
    lastTime = now;

    // Check if any events have been activated (key pressed, mouse moved etc.)
    // and call corresponding response functions
    glfwPollEvents();

    scene.UpdateData(deltaTime);

    // Render
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(cubeShaderProgram);

    // Pass VP, camera position
    glm::mat4 VP = Scene::get_camera().ViewProjectionMatrix();
    glUniformMatrix4fv(VP_location, 1, false, glm::value_ptr(VP));
    glUniform3f(cam_pos_location, Scene::get_camera().position.x, Scene::get_camera().position.y, Scene::get_camera().position.z);

    Scene::DrawScene(scene.get_root(), cubeShaderProgram);
    Scene::DrawScene(scene.get_player(), cubeShaderProgram);
    Scene::DrawScene(scene.get_lamp(), cubeShaderProgram);
    Scene::DrawScene(scene.get_hud(), hudShaderProgram);

    // Swap the screen buffers
    glfwSwapBuffers(Scene::getWindow());
  }

  delete player;
  delete sphere;
  delete lamp;

  glDeleteProgram(cubeShaderProgram);
  glDeleteProgram(hudShaderProgram);

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
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

  // Create a GLFWwindow object that we can use for GLFW's functions
  GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GFX Playground",nullptr, NULL);
  glfwMakeContextCurrent(window);
  if (window == NULL) {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return false;
  }

  Scene::setWindow(window);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

  OpenglUtils::initOpenglDebugOutput();
  return true;
}
