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

int level ;

// Start our window
bool InitWindow();

// ====================================================================================================
int main()
{
    if (!InitWindow())
        return -1;
    
    // Compile and link the shader program
    GLuint cubeShaderProgram = Shader::LoadShader("../shaders/cube.vert", "../shaders/color.frag");
    GLuint hudShaderProgram = Shader::LoadShader("../shaders/font.vert", "../shaders/font.frag");
    Scene::VP_location = glGetUniformLocation(cubeShaderProgram, "VP");

    // Create the font
    //Font nsm("../fonts/nsm.ttf");
    Font airstrike("../fonts/airstrikebold.ttf");

    // Create the mesh
    Mesh* cube = Mesh_Utils::WhiteCube();
    Mesh* PRISMA = Mesh_Utils::TextMesh("PRISMA", &airstrike);
    Mesh* Level1 = Mesh_Utils::TextMesh("Level 1", &airstrike);
    Mesh* Level2 = Mesh_Utils::TextMesh("Level 2", &airstrike);
    Scene::InitScene(cube);  
    
        // HUD:
        Scene::hud = new Scene_Node;
        Scene::hud->relativeModel = glm::translate(glm::mat4(1), glm::vec3(0, -0.9f, 0));

        glm::mat4 Model;
        Scene_Node* prismatext = new Scene_Node(PRISMA);
        Model = glm::translate(glm::mat4(1), glm::vec3(-0.4f, 1.4f, 0));
        prismatext->absoluteScale = glm::vec3(0.08f);
        prismatext->relativeModel = Model;
        prismatext->color = glm::vec4(0.1,0.5f,0.5f,1);
        Scene::hud->AddChild(prismatext);


        glm::mat4 Model1;
        Scene_Node* level1text = new Scene_Node(Level1);
        Model1 = glm::translate(glm::mat4(1), glm::vec3(-0.4f, 0.8, 0));
        level1text->absoluteScale = glm::vec3(0.08f);
        level1text->relativeModel = Model1;
        level1text->color = glm::vec4(0,0.f,1.f,1);
        Scene::hud->AddChild(level1text);

        glm::mat4 Model2;
        Scene_Node* level2text = new Scene_Node(Level2);
        Model2 = glm::translate(glm::mat4(1), glm::vec3(-0.4f, 0.4, 0));
        level2text->absoluteScale = glm::vec3(0.08f);
        level2text->relativeModel = Model2;
        level2text->color = glm::vec4(1.f,1.f,1.f,1);
        Scene::hud->AddChild(level2text);


    
    glUseProgram(hudShaderProgram);

    

    // Load the font, just like a texture
    Scene::texture_sampler_location = glGetUniformLocation(hudShaderProgram, "texture_sampler");
    //nsm.Bind(0);
    airstrike.Bind(0);
    glUniform1i(Scene::texture_sampler_location, 0);
    
    // Create the camera object
    Scene::camera.aspectRatio = (float)WIDTH / HEIGHT;

    // Game loop 
    int count = 0;
    while (!glfwWindowShouldClose(Scene::window))
    {
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        Scene::UpdateData();

        // Render
        // Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(cubeShaderProgram);
       // glm::vec3(primaryColor->color) += glm::vec3(0.01f,0.01f,0.01f);
       if(count == 250)
       {
            prismatext->color = glm::vec4(0.2f,0.5,0.7f,1);
            count = 0;
       }
       else
       {
            prismatext->color += glm::vec4(0.005f,0.f,0.003f,0);
       }
       count++;
        // Pass the VP matrix to the main shader
        glm::mat4 VP = Scene::camera.ViewProjectionMatrix();
        glUniformMatrix4fv(Scene::VP_location, 1, false, glm::value_ptr(VP));

        //Scene::DrawScene(Scene::room, cubeShaderProgram); 
        //Scene::DrawScene(Scene::player, cubeShaderProgram); 
        Scene::DrawScene(Scene::hud, hudShaderProgram);
       
        
        // Swap the screen buffers
        glfwSwapBuffers(Scene::window);
    }

    glfwTerminate();

    delete cube;
    delete PRISMA;
    delete Level1;
    delete Level2;

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
    glDepthFunc(GL_LEQUAL);

    // Anti-aliasing
    glEnable(GL_LINE_SMOOTH);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBlendEquation(GL_FUNC_ADD);

    return true;
}

#include "scene/scene.cpp"
