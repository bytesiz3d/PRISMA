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

// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, int button, int action, int mods);

// Window dimensions
const GLuint WIDTH = 1280, HEIGHT = 720;

// Collision
bool TestCollision_Cubes(glm::mat4 objectA, glm::mat4 objectB);

namespace Scene
{
    GLFWwindow* window;

    // Controls:
    double p_mX, p_mY,
           mX,   mY;

    glm::vec3 movementP(0.f);
    glm::vec3 movementN(0.f);
    glm::vec2 mouseDelta(0.f);
    bool holdMouse = false;

    // Utility functions:
    void UpdateData();
    void InitScene(Mesh* cc);
    void DrawScene(Scene_Node* scene, GLuint shaderId);

    // Camera:
    Camera camera;
    float cameraAngle = 0.f;
    GLint VP_location;

    // Room:
    Scene_Node* room;

    // Player:
    Scene_Node* player;
    Scene_Node* orb;
    Scene_Node* door;

    // HUD circles:
    Scene_Node* hud;
    bool lastState = false;
};

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
    glfwWindowHint(GLFW_SAMPLES, 16);

    // Create a GLFWwindow object that we can use for GLFW's functions
    Scene::window = glfwCreateWindow(WIDTH, HEIGHT, "GFX Playground", NULL, NULL);
    glfwMakeContextCurrent(Scene::window);
    if (Scene::window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set the required callback functions
    glfwSetKeyCallback(Scene::window, KeyCallback);
    glfwSetMouseButtonCallback(Scene::window, MouseCallback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }
    
    // Compile and link the shader program
    GLuint cubeShaderProgram = Shader::LoadShader("../shaders/cube.vert", "../shaders/color.frag");
    // GLuint cubeShaderProgram = Shader::LoadShader("../shaders/color.vert", "../shaders/color.frag");
    GLuint hudShaderProgram = Shader::LoadShader("../shaders/hud.vert", "../shaders/color.frag");
    // glUseProgram(cubeShaderProgram);

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    // Anti-aliasing:
    // glEnable(GL_LINE_SMOOTH);

    // glLineWidth(5.f);

    // Get uniform variable locations
    Scene::VP_location = glGetUniformLocation(cubeShaderProgram, "VP");
    
    // Create the mesh
    Mesh cube = Mesh_Utils::WhiteCube();
    Scene::InitScene(&cube);
    
    // Create the camera object:
    {
        // Look at the player:
        glm::vec4 p = Scene::player->relativeModel * glm::vec4(0.f, 0.f, 0.f, 1.f);

        Scene::camera.position = glm::vec3(p) + glm::vec3(0, 24, 32);
        Scene::camera.aspectRatio = (float)WIDTH / HEIGHT;
        Scene::camera.SetTarget(glm::vec3(p));
    }

    // Black background
    glClearColor(0.f, 0.f, 0.f, 1.f);

    // Set the depth test function:
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    // // Enable blending:
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glBlendEquation(GL_FUNC_ADD);

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

        // Pass the VP matrix to the shader
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

void Scene::UpdateData()
{
    glfwGetCursorPos(window, &mX, &mY);
    mouseDelta = glm::vec2(mX - p_mX, mY - p_mY);
    p_mX = mX, p_mY = mY;

    // Move player:
    if (holdMouse)
    {
        glm::vec3 dm = movementP - movementN;
        camera.UpdateCamera2D(mouseDelta, dm);
        // camera.UpdateCamera(mouseDelta, dm);

        glm::vec3 pos = camera.position;
        pos[1] = 0;
            
        glm::vec3 forward = camera.direction;
        forward[1] = 0;
        forward = glm::normalize(forward);

        pos += 32.f * forward; // Distance from camera

        if (TestCollision_Cubes(player->ScaleWorldModel(), door->ScaleWorldModel()))
        {
            if (player->color != door->color)
            {
                std::cout << "debug: hit\n";
                // Reverse the move:
                camera.UpdateCamera2D(mouseDelta, -dm);
                pos -= 32.f * forward;
            }
        }
        player->relativeModel = glm::translate(glm::mat4(1.f), pos);

        // Align object:
        cameraAngle -= 0.001 * mouseDelta[0];
        player->relativeModel = glm::rotate(player->relativeModel, cameraAngle, glm::vec3(0, 1, 0));
    }

    // Check for collision with the orb:
    bool currentState = TestCollision_Cubes(player->ScaleWorldModel(), orb->ScaleWorldModel());
    if (currentState != lastState && currentState)
    {
        // Swap colors
        glm::vec4 newPlayerColor = orb->color;
        orb->color = player->color;
        player->color = newPlayerColor;

        // Update HUD
        hud->children[0]->color = newPlayerColor;
    }
    lastState = currentState;

}

// ====================================================================================================
void Scene::InitScene(Mesh* cc)
{
    glm::mat4 Model;
    room = new Scene_Node;

    // Player
    player = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1.f), glm::vec3(-64, 0, 0));
    player->relativeModel = Model;
    player->absoluteScale = glm::vec3(16, 16, 5);
    player->color = glm::vec4(0.f, 1.f, 0.f, 1.f);
    player->drawMode = GL_LINE_LOOP;
        
    // Right face:
    Scene_Node* right = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1.f), glm::vec3(128, 0, 0));
    right->relativeModel = Model;
    right->absoluteScale = glm::vec3(000, 256, 256); 
    room->AddChild(right);
    
    // Left face:
    Scene_Node* left = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1.f), glm::vec3(-128, 0, 0));
    left->relativeModel = Model;
    left->absoluteScale = glm::vec3(000, 256, 256); 
    room->AddChild(left);

    // Roof face:
    Scene_Node* roof = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1.f), glm::vec3(0, 256, 0));
    roof->relativeModel = Model;
    roof->absoluteScale = glm::vec3(256, 000, 256);
    roof->color = glm::vec4(5.f/255, 42.f/255, 40.f/255, 1.f);
    room->AddChild(roof);
    
    // Floor face:
    Scene_Node* floor = new Scene_Node(cc);
    floor->absoluteScale = glm::vec3(256, 000, 256);
    floor->color = glm::vec4(5.f/255, 42.f/255, 40.f/255, 1.f);
    room->AddChild(floor);

    // Front face:
    Scene_Node* front = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -128));
    front->relativeModel = Model;
    front->absoluteScale = glm::vec3(256, 256, 000);
    front->color = glm::vec4(21.f/255, 5.f/255, 42.f/255, 1.f);
    room->AddChild(front);

    // Back face:
    Scene_Node* back = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 128));
    back->relativeModel = Model;
    back->absoluteScale = glm::vec3(256, 256, 000);
    back->color = glm::vec4(21.f/255, 5.f/255, 42.f/255, 1.f);
    room->AddChild(back);

    // Door:
    door = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1.f), glm::vec3(128, 0, 0));
    door->relativeModel = Model;
    door->absoluteScale = glm::vec3(1, 128, 64);
    door->color = glm::vec4(1.f, 0.f, 0.f, 1.f);
    room->AddChild(door);
    
    // Orb:
    orb = new Scene_Node(cc);
    // Scene_Node* orb = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1.f), glm::vec3(64, 0, 0));
    orb->relativeModel = Model;
    orb->absoluteScale = glm::vec3(16);
    orb->color = glm::vec4(1.f, 0.f, 0.f, 1.f);
    room->AddChild(orb);

    // HUD:
    hud = new Scene_Node;
    hud->relativeModel = glm::translate(glm::mat4(1.f), glm::vec3(0, -0.9f, 0));

    Scene_Node* primaryColor = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1.f), glm::vec3(0.75f, 0, 0));
    primaryColor->relativeModel = Model;
    primaryColor->color = player->color;
    hud->AddChild(primaryColor);

    Scene_Node* secondaryColor = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1.f), glm::vec3(0.85f, 0, 0));
    secondaryColor->relativeModel = Model;
    hud->AddChild(secondaryColor);
}

void Scene::DrawScene(Scene_Node* scene, GLuint shaderId)
{
    if (scene)
    {
        scene->Draw(shaderId);
        for (auto& child : scene->children)
            DrawScene(child, shaderId);
    }
}

// ====================================================================================================
bool TestCollision_Cubes(glm::mat4 objectA, glm::mat4 objectB)
{
    // AABB:
    glm::vec4 objectA_center = objectA * glm::vec4(0.f, 0.f, 0.f, 1.f);
    glm::vec4 objectB_center = objectB * glm::vec4(0.f, 0.f, 0.f, 1.f);
    glm::vec3 objectA_size;
    glm::vec3 objectB_size;

    float objectA_t;
    float objectB_t;
    
    // X-size:
    objectA_t = (objectA_center - (objectA * glm::vec4(0.5f, 0.f, 0.f, 1.f)))[0];
    objectA_size[0] = objectA_t;
    objectB_t = (objectB_center - (objectB * glm::vec4(0.5f, 0.f, 0.f, 1.f)))[0]; 
    objectB_size[0] = objectB_t;
    
    // Y-size:
    objectA_t = (objectA_center - (objectA * glm::vec4(0.f, 0.5f, 0.f, 1.f)))[1];
    objectA_size[1] = objectA_t;
    objectB_t = (objectB_center - (objectB * glm::vec4(0.f, 0.5f, 0.f, 1.f)))[1]; 
    objectB_size[1] = objectB_t;
    
    // Z-size:
    objectA_t = (objectA_center - (objectA * glm::vec4(0.f, 0.f, 0.5f, 1.f)))[2];
    objectA_size[2] = objectA_t;
    objectB_t = (objectB_center - (objectB * glm::vec4(0.f, 0.f, 0.5f, 1.f)))[2]; 
    objectB_size[2] = objectB_t;
    
    return (
        glm::abs(objectA_center[0] - objectB_center[0]) <= glm::abs(objectA_size[0] + objectB_size[0]) &&
        glm::abs(objectA_center[1] - objectB_center[1]) <= glm::abs(objectA_size[1] + objectB_size[1]) &&
        glm::abs(objectA_center[2] - objectB_center[2]) <= glm::abs(objectA_size[2] + objectB_size[2])
    );
}

// ====================================================================================================
// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
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
    if (button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
            Scene::holdMouse = true;
        else if (action == GLFW_RELEASE)
            Scene::holdMouse = false;
    }
}
