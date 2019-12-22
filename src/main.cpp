#include <iostream>
#include <fstream>
#include <vector>

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "shader/shader.hpp"
#include "camera/camera.hpp"

#include "scene_node/scene_node.hpp"
#include "player/player.hpp"
#include "orb/orb.hpp"

#include "mesh/mesh_utils.hpp"
#include "texture/texture.hpp"
#include "font/font.hpp"

// *NOT SEPARATELY BUILT*
// TODO: Add shaderIDs to the JSON file
#include "scene/scene.hpp"

// Window dimensions
const GLuint WIDTH = 1280, HEIGHT = 720;

// Start our window
bool InitWindow();

struct Light
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 direction;
    glm::vec3 position;
    float attenuation;

} light;

// ====================================================================================================
int main()
{
    if (!InitWindow())
        return -1;
    
    // Compile and link the shader program
    //GLuint cubeShaderProgram = Shader::LoadShader("../shaders/texture.vert", "../shaders/texture.frag");
    GLuint cubeShaderProgram = Shader::LoadShader("../shaders/light.vert", "../shaders/light.frag");
    GLuint hudShaderProgram = Shader::LoadShader("../shaders/hud.vert", "../shaders/color.frag");

    glUseProgram(cubeShaderProgram);
    Scene::VP_location = glGetUniformLocation(cubeShaderProgram, "VP");
    Scene::texture_sampler_location = glGetUniformLocation(cubeShaderProgram, "texture_sampler");

    // Create meshes
    Mesh* cube = Mesh_Utils::WhiteCube();
    Mesh* player = Mesh_Utils::OBJMesh("../res/models/player");
    Scene::meshes[MESH_CUBE] = cube;
    Scene::meshes[MESH_MODEL0] = player;

    // Create textures
    Texture grid("../res/textures/tiles-256.png", GL_RGB);
    Texture wood("../res/textures/albedo.jpg", GL_RGB);
    Texture white;
    Scene::textures[MESH_TEXTURE_NULL] = &white;
    Scene::textures[MESH_TEXTURE0] = &wood;
    Scene::textures[MESH_TEXTURE1] = &grid;

    // Initialize scene
    Scene::InitScene("../res/scenes/room.json");

    // Create the camera object
    Scene::camera.aspectRatio = (float)WIDTH / HEIGHT;

    // Setup lights
    light.ambient = { 0.1, 0.1, 0.1 };
    light.diffuse = { 1, 1, 1 };
    light.specular = { 1, 1, 1 };
    light.direction = { -1, -1, -1 };
    light.direction = glm::normalize(light.direction);
    light.position = { 0, 64, 0 };
    light.attenuation = std::pow(2, -12);

    int light_amb = glGetUniformLocation(cubeShaderProgram, "light.ambient");
    int light_diff = glGetUniformLocation(cubeShaderProgram, "light.diffuse");
    int light_spec = glGetUniformLocation(cubeShaderProgram, "light.specular");
    int light_dir = glGetUniformLocation(cubeShaderProgram, "light.direction");
    int light_pos = glGetUniformLocation(cubeShaderProgram, "light.position");
    int light_att = glGetUniformLocation(cubeShaderProgram, "light.attenuation");

    glUniform3f(light_amb, light.ambient.r, light.ambient.g, light.ambient.b);
    glUniform3f(light_diff, light.diffuse.r, light.diffuse.g, light.diffuse.b);
    glUniform3f(light_spec, light.specular.r, light.specular.g, light.specular.b);
    glUniform3f(light_dir, light.direction.x, light.direction.y, light.direction.z);
    glUniform3f(light_pos, light.position.x, light.position.y, light.position.z);
    glUniform1f(light_att, light.attenuation);

    int cam_pos = glGetUniformLocation(cubeShaderProgram, "cam_position");

    glUniform1f(Scene::texture_sampler_location, 0);
    // Game loop
    while (!glfwWindowShouldClose(Scene::window))
    {
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        Scene::UpdateData();

        // Render
        // Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(cubeShaderProgram);

        // Pass VP, camera position
        glm::mat4 VP = Scene::camera.ViewProjectionMatrix();
        glUniformMatrix4fv(Scene::VP_location, 1, false, glm::value_ptr(VP));
        glUniform3f(cam_pos, Scene::camera.position.x, Scene::camera.position.y, Scene::camera.position.z);

        Scene::DrawScene(Scene::root, cubeShaderProgram); 
        Scene::DrawScene(Scene::player, cubeShaderProgram); 
        Scene::DrawScene(Scene::hud, hudShaderProgram);
        
        // Swap the screen buffers
        glfwSwapBuffers(Scene::window);
    }

    glfwTerminate();

    delete cube;
    delete player;
    //delete sphere;

    glDeleteProgram(cubeShaderProgram);
    glDeleteProgram(hudShaderProgram);

    Scene::DeleteAllPointers();
    return 0;
}

// ====================================================================================================
bool InitWindow()
{
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;

    // Init GLFW
    glfwInit();

    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_DEPTH_BITS, 32);
    glfwWindowHint(GLFW_SAMPLES, 16);

    // Create a GLFWwindow object that we can use for GLFW's functions
    Scene::window = glfwCreateWindow(WIDTH, HEIGHT, "GFX Playground", NULL, NULL);
    glfwMakeContextCurrent(Scene::window);
    if (Scene::window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    // Set the required callback functions
    glfwSetKeyCallback(Scene::window, Scene::KeyCallback);
    glfwSetMouseButtonCallback(Scene::window, Scene::MouseCallback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
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
