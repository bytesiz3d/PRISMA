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

// Window dimensions
GLuint WIDTH = 1280, HEIGHT = 720;
int level = -1;

// Start our window
bool InitWindow();

void MainMenu();
void RunGame();

// ====================================================================================================
int main() {
  if (!InitWindow())
    return -1;
  MainMenu();
  RunGame();
  glfwTerminate();
  return 0;
}

// ====================================================================================================
void MainMenu() {
  glDisable(GL_CULL_FACE);

  // Compile and link the shader program
  GLuint shaderProgram = Shader::LoadShader("../shaders/font.vert", "../shaders/font.frag");

  // Create the font
  Font font("../fonts/airstrikebold.ttf");

  // Create the mesh
  Mesh* PRISMA = Mesh_Utils::TextMesh("PRISMA", &font);
  Mesh* Level1 = Mesh_Utils::TextMesh("Level 1", &font);
  Mesh* Level2 = Mesh_Utils::TextMesh("Level 2", &font);

  // HUD:
  Scene_Node* hud = new Scene_Node;
  hud->relativeModel = glm::translate(glm::mat4(1), glm::vec3(0, -0.9f, 0));

  glm::mat4 Model;
  Scene_Node* prismatext = new Scene_Node(PRISMA);
  Model = glm::translate(glm::mat4(1), glm::vec3(-0.4f, 1.4f, 0));
  prismatext->absoluteScale = glm::vec3(0.08f);
  prismatext->relativeModel = Model;
  prismatext->color = glm::vec4(0.1, 0.5f, 0.5f, 1);
  hud->AddChild(prismatext);

  glm::mat4 Model1;
  Scene_Node* level1text = new Scene_Node(Level1);
  Model1 = glm::translate(glm::mat4(1), glm::vec3(-0.4f, 0.8, 0));
  level1text->absoluteScale = glm::vec3(0.08f);
  level1text->relativeModel = Model1;
  level1text->color = glm::vec4(1.f, 1.f, 1.f, 1);
  hud->AddChild(level1text);

  glm::mat4 Model2;
  Scene_Node* level2text = new Scene_Node(Level2);
  Model2 = glm::translate(glm::mat4(1), glm::vec3(-0.4f, 0.4, 0));
  level2text->absoluteScale = glm::vec3(0.08f);
  level2text->relativeModel = Model2;
  level2text->color = glm::vec4(1.f, 1.f, 1.f, 1);
  hud->AddChild(level2text);

  std::array levels = {level1text, level2text};

  glUseProgram(shaderProgram);

  // Load the font, just like a texture
  const GLint texture_sampler_location = glGetUniformLocation(shaderProgram, "texture_sampler");
  font.Bind(0);
  glUniform1i(texture_sampler_location, 0);

  int count = 0;
  // Game loop
  while (!glfwWindowShouldClose(Scene::getWindow())) {
    // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
    glfwPollEvents();

    auto [exit, startGame, level] = InputManager::ProcessMainMenuInput();
    if (exit) {
      glfwSetWindowShouldClose(Scene::getWindow(), true);
    }

    if (startGame) {
      ::level = level;
      break;  // Exit the main menu loop and start the game
    }

    for (auto & l : levels) {
      l->color = glm::vec4(1.f, 1.f, 1.f, 1);
    }

    levels[level-1]->color = glm::vec4(0.2f, 0.5f, 0.7f, 1);

    // Render
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderProgram);

    if (count == 250) {
      prismatext->color = glm::vec4(0.2f, 0.5, 0.7f, 1);
      count = 0;
    }
    else {
      prismatext->color += glm::vec4(0.005f, 0.f, 0.003f, 0);
    }
    count++;
    Scene::DrawScene(hud, shaderProgram);

    // Swap the screen buffers
    glfwSwapBuffers(Scene::getWindow());
  }

  glDeleteProgram(shaderProgram);
  delete PRISMA;
  delete Level1;
  delete Level2;
}

// ====================================================================================================
void RunGame() {
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
  Mesh* cube = Mesh_Utils::WhiteCube();
  Mesh* sphere = Mesh_Utils::Sphere();
  Mesh* player = Mesh_Utils::LoadMesh("../res/models/player.assbin");
  Mesh* lamp = Mesh_Utils::LoadMesh("../res/models/lamp.assbin");
  scene.set_meshes({
    {MESH_NULL, nullptr},
    {MESH_CUBE, cube},
    {MESH_SPHERE, sphere},
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
  if (level == 1)
    scene.InitScene("../res/scenes/level1.json");
  else if (level == 2)
    scene.InitScene("../res/scenes/level2.json");

  scene.UploadLights(cubeShaderProgram);

  glUniform1i(texture_sampler_location, 0);
  // Game loop
  while (!glfwWindowShouldClose(Scene::getWindow())) {
    // Check if any events have been activated (key pressed, mouse moved etc.)
    // and call corresponding response functions
    glfwPollEvents();

    scene.UpdateData();

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

  delete cube;
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
