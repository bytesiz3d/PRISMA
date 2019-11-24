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

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;

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

    // Get uniform variable locations
    int MVP_location = glGetUniformLocation(shaderProgram, "MVP");
    int tint_location = glGetUniformLocation(shaderProgram, "tint");
    
    // Create the mesh
    Mesh sphere = Mesh_Utils::ColoredSphere();
    
    // Scale the sphere down x0.25
    glm::mat4 MVP(1.f); //Identity matrix
    MVP = glm::scale(MVP, glm::vec3(0.25, 0.25, 0.25));
    
    // Change the color to white
    glm::mat4 tint(0.f); //Zero matrix
    tint[3] = glm::vec4(1.f); //Last column = 1, 1, 1, 1

    // Set uniform variables
    glUniformMatrix4fv(MVP_location, 1, false, glm::value_ptr(MVP));
    glUniformMatrix4fv(tint_location, 1, false, glm::value_ptr(tint));

    // Black background
    glClearColor(0.f, 0.f, 0.f, 1.f);
    
    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the color buffer
        glClear(GL_COLOR_BUFFER_BIT);
        sphere.draw();

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    std::cout << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
