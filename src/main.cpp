#include <iostream>

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
#include "mesh/mesh_utils.hpp"
#include "camera/camera.hpp"
#include "scene_node/scene_node.hpp"
#include "player/player.hpp"
#include "texture/texture.hpp"

// *NOT SEPARATELY BUILT*
#include "scene/scene.hpp"

// Window dimensions
const GLuint WIDTH = 1280, HEIGHT = 720;

// Start our window
bool InitWindow();

// ====================================================================================================
int main()
{
    if (!InitWindow())
        return -1;
    
    // Compile and link the shader program
    GLuint cubeShaderProgram = Shader::LoadShader("../shaders/cube.vert", "../shaders/color.frag");
    GLuint hudShaderProgram = Shader::LoadShader("../shaders/hud.vert", "../shaders/color.frag");
    Scene::VP_location = glGetUniformLocation(cubeShaderProgram, "VP");

    // Create the mesh
    //Mesh cube = Mesh_Utils::WhiteCube();
    Mesh cube = Mesh_Utils::OBJMesh("../models/suzanne");
    Scene::InitScene(&cube);
    
    // Create the camera object
    Scene::camera.aspectRatio = (float)WIDTH / HEIGHT;

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

        // Pass the VP matrix to the main shader
        glm::mat4 VP = Scene::camera.ViewProjectionMatrix();
        glUniformMatrix4fv(Scene::VP_location, 1, false, glm::value_ptr(VP));

        Scene::DrawScene(Scene::room, cubeShaderProgram); 
        Scene::DrawScene(Scene::player, cubeShaderProgram); 
        Scene::DrawScene(Scene::hud, hudShaderProgram);
        
        // Swap the screen buffers
        glfwSwapBuffers(Scene::window);
    }

    glfwTerminate();
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
    glDepthFunc(GL_LEQUAL);

    // // Anti-aliasing
    // glEnable(GL_LINE_SMOOTH);

    // // Enable blending
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glBlendEquation(GL_FUNC_ADD);

    return true;
}

#include "scene/scene.cpp"
