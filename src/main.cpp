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

#include "shader/shader.hpp"
#include "mesh/mesh_utils.hpp"
#include "camera/camera.hpp"

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, int button, int action, int mods);
void DrawScene(Mesh* cc);

// Window dimensions
const GLuint WIDTH = 1600, HEIGHT = 900;

namespace Scene
{
    double p_mX, p_mY,
           mX,   mY;

    glm::vec3 movementP(0.f);
    glm::vec3 movementN(0.f);
    glm::vec2 mouseDelta(0.f);
    bool holdMouse = false;

    GLint M_location;
    GLint VP_location;
    GLint tint_location;
};

// The MAIN function, from here we start the application and run the game loop
int main()
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

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "GFX Playground", NULL, NULL);
    glfwMakeContextCurrent(window);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetMouseButtonCallback(window, MouseCallback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }
    
    // Compile and link the shader program
    GLuint shaderProgram = Shader::LoadShader("../shaders/color.vert", "../shaders/color.frag");
    glUseProgram(shaderProgram);

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    // Create the camera object:
    Camera camera;
    camera.position = glm::vec3(200, 200, 000);
    camera.aspectRatio = (float)WIDTH / HEIGHT;
    camera.SetTarget(glm::vec3(0, 0, 0));

    // Get uniform variable locations
    Scene::M_location = glGetUniformLocation(shaderProgram, "M");
    Scene::VP_location = glGetUniformLocation(shaderProgram, "VP");
    Scene::tint_location = glGetUniformLocation(shaderProgram, "tint");
    
    // Create the mesh
    Mesh cube = Mesh_Utils::WhiteCube();

    // Black background
    glClearColor(0.f, 0.f, 0.f, 1.f);

    // Set the depth test function:
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        glfwGetCursorPos(window, &Scene::mX, &Scene::mY);
        Scene::mouseDelta = glm::vec2(Scene::mX - Scene::p_mX, Scene::mY - Scene::p_mY);
        Scene::p_mX = Scene::mX, Scene::p_mY = Scene::mY;

        // TODO: Patch up the camera controller (qe, ad, ws)
        if (Scene::holdMouse)
            camera.UpdateCamera(Scene::mouseDelta, Scene::movementP - Scene::movementN);

        // Render
        // Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        // Pass the VP matrix to the shader
        glm::mat4 VP = camera.ViewProjectionMatrix();
        glUniformMatrix4fv(Scene::VP_location, 1, false, glm::value_ptr(VP));

        DrawScene(&cube);
        
        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_W:
            Scene::movementP[2] = 1;
            break;
        case GLFW_KEY_S:
            Scene::movementN[2] = 1;
            break;

        case GLFW_KEY_D:
            Scene::movementP[0] = 1;
            break;
        case GLFW_KEY_A:
            Scene::movementN[0] = 1;
            break;

        case GLFW_KEY_Q:
            Scene::movementP[1] = 1;
            break;
        case GLFW_KEY_E:
            Scene::movementN[1] = 1;
            break;

        default:
            break;
        }
    }

    if (action == GLFW_RELEASE)
    {
        switch (key)
        {
        case GLFW_KEY_W:
            Scene::movementP[2] = 0;
            break;
        case GLFW_KEY_S:
            Scene::movementN[2] = 0;
            break;

        case GLFW_KEY_D:
            Scene::movementP[0] = 0;
            break;
        case GLFW_KEY_A:
            Scene::movementN[0] = 0;
            break;

        case GLFW_KEY_Q:
            Scene::movementP[1] = 0;
            break;
        case GLFW_KEY_E:
            Scene::movementN[1] = 0;
            break;

        default:
            break;
        }
    }
}

void MouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
            Scene::holdMouse = true;
        else if (action == GLFW_RELEASE)
            Scene::holdMouse = false;
    }
}
void DrawScene(Mesh* cc)
{
    glm::mat4 Model;
    glm::mat4 tint;
    // Room:
    // Cube of size 256, located at the origin:
    // Three pairs of faces:
    
    {
        // Right face:
        Model = glm::translate(glm::mat4(1.f), glm::vec3(128, 0, 0));
        Model = glm::scale(Model, glm::vec3(000, 256, 256));
        glUniformMatrix4fv(Scene::M_location, 1, false, glm::value_ptr(Model));
        // White:
        tint = glm::mat4(1.f);
        glUniformMatrix4fv(Scene::tint_location, 1, false, glm::value_ptr(tint));
        cc->draw();

        // Left face:
        Model = glm::translate(glm::mat4(1.f), glm::vec3(-128, 0, 0));
        Model = glm::scale(Model, glm::vec3(000, 256, 256));
        glUniformMatrix4fv(Scene::M_location, 1, false, glm::value_ptr(Model));
    
        cc->draw();
    }
    {

        // Roof face:
        Model = glm::translate(glm::mat4(1.f), glm::vec3(0, 128, 0));
        Model = glm::scale(Model, glm::vec3(256, 000, 256));
        glUniformMatrix4fv(Scene::M_location, 1, false, glm::value_ptr(Model));
        // color:
        tint = glm::mat4(0.f);
        tint[3] = glm::vec4(5.f/255, 42.f/255, 40.f/255, 1.f);
        glUniformMatrix4fv(Scene::tint_location, 1, false, glm::value_ptr(tint));
        cc->draw();

        // Floor face:
        Model = glm::translate(glm::mat4(1.f), glm::vec3(0, -128, 0));
        Model = glm::scale(Model, glm::vec3(256, 000, 256));
        glUniformMatrix4fv(Scene::M_location, 1, false, glm::value_ptr(Model));
    
        cc->draw();

    }
    {
        // Front face:
        Model = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -128));
        Model = glm::scale(Model, glm::vec3(256, 256, 000));
        glUniformMatrix4fv(Scene::M_location, 1, false, glm::value_ptr(Model));
        // color:
        tint = glm::mat4(0.f);
        tint[3] = glm::vec4(21.f/255, 5.f/255, 42.f/255, 1.f);
        glUniformMatrix4fv(Scene::tint_location, 1, false, glm::value_ptr(tint));
        cc->draw();

        // Back face:
        Model = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 128));
        Model = glm::scale(Model, glm::vec3(256, 256, 000));
        glUniformMatrix4fv(Scene::M_location, 1, false, glm::value_ptr(Model));
    
        cc->draw();
    }

    // Door:
    // Square of size 64 x 128 (z * y)
    // Shifted 128 units to the right, 64 units downwards:
    Model = glm::mat4(1.f);
    // Model = glm::translate(Model, glm::vec3(-16, 10, 0)); // Z-fighting!!!!!
    Model = glm::translate(Model, glm::vec3(128, -64, 0));
    Model = glm::scale(Model, glm::vec3(8, 128, 64));
    glUniformMatrix4fv(Scene::M_location, 1, false, glm::value_ptr(Model));

    // Red 
    tint = glm::mat4(0.f);
    tint[0][0] = 1.f;
    tint[3][3] = 1.f;
    glUniformMatrix4fv(Scene::tint_location, 1, false, glm::value_ptr(tint));

    cc->draw();

    // Player:
    // Square of size 32 (x * y)
    // Shifted 64 units to the left, 112 units downwards:
    Model = glm::mat4(1.f);
    // Model = glm::translate(Model, glm::vec3(0, 10, 0)); // Z-fighting!!!!!
    Model = glm::translate(Model, glm::vec3(-64, -112, 0));
    Model = glm::scale(Model, glm::vec3(32, 32, 5));
    glUniformMatrix4fv(Scene::M_location, 1, false, glm::value_ptr(Model));

    // Blue 
    tint = glm::mat4(0.f);
    tint[2][2] = 1.f;
    tint[3][3] = 1.f;
    glUniformMatrix4fv(Scene::tint_location, 1, false, glm::value_ptr(tint));

    cc->draw();

    // Orb:
    // Cube of size 32
    // Shifted 64 units to the right, 112 units downwards:
    Model = glm::mat4(1.f);
    // Model = glm::translate(Model, glm::vec3(0, 10, 0)); // Z-fighting!!!!!
    Model = glm::translate(Model, glm::vec3(64, -112, 0));
    Model = glm::scale(Model, glm::vec3(32, 32, 32));
    glUniformMatrix4fv(Scene::M_location, 1, false, glm::value_ptr(Model));

    // Green 
    tint = glm::mat4(0.f);
    tint[1][1] = 1.f;
    tint[3][3] = 1.f;
    glUniformMatrix4fv(Scene::tint_location, 1, false, glm::value_ptr(tint));

    cc->draw();
}
