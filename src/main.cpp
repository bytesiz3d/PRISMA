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

    glm::vec3 movementP(0);
    glm::vec3 movementN(0);
    glm::vec2 mouseDelta(0);
    bool holdMouse = false;

    // Utility functions:
    void UpdateData();
    void InitScene(Mesh* cc);
    void DrawScene(Scene_Node* scene, GLuint shaderId);
    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
    void MouseCallback(GLFWwindow* window, int button, int action, int mods);

    // Camera:
    Camera camera;
    float cameraAngle = 0.f;
    GLint VP_location;

    // Room:
    Scene_Node* room;

    // Player:
    Player* player;
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
    glfwSetKeyCallback(Scene::window, Scene::KeyCallback);
    glfwSetMouseButtonCallback(Scene::window, Scene::MouseCallback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }
    
    // Compile and link the shader program
    GLuint cubeShaderProgram = Shader::LoadShader("../shaders/cube.vert", "../shaders/color.frag");
    GLuint hudShaderProgram = Shader::LoadShader("../shaders/hud.vert", "../shaders/color.frag");

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
        glm::vec3 p = Scene::player->position;

        Scene::camera.position = p + glm::vec3(-32, 24, 0);
        Scene::camera.aspectRatio = (float)WIDTH / HEIGHT;
        Scene::camera.SetTarget(p);
    }

    // Black background
    glClearColor(0, 0, 0, 1);

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

void Scene::UpdateData()
{
    glfwGetCursorPos(window, &mX, &mY);
    mouseDelta = glm::vec2(mX - p_mX, mY - p_mY);
    p_mX = mX, p_mY = mY;

    if (!holdMouse)
        mouseDelta = glm::vec2(0);

    // Move player:
    glm::vec3 dm = movementP - movementN;
    player->UpdatePlayer(mouseDelta[0], dm);

    // Manually updating the camera
    camera.position = player->position;
    camera.position[1] += 24;
    camera.position -= 32.f * player->direction;
    camera.SetTarget(player->position);
    
    // camera.UpdateCamera2D(mouseDelta, dm);
    // camera.UpdateCamera(mouseDelta, dm);

    // glm::vec3 pos = camera.position;
    // pos[1] = 0;
            
    // glm::vec3 forward = camera.direction;
    // forward[1] = 0;
    // forward = glm::normalize(forward);

    // pos += 32.f * forward; // Distance from camera

    // player->relativeModel = glm::translate(glm::mat4(1), pos);

    // // Align object with camera
    // cameraAngle -= 0.001 * mouseDelta[0];
    // player->relativeModel = glm::rotate(player->relativeModel, cameraAngle, glm::vec3(0, 1, 0));

    // TODO: Fix collision with the door
    // if (TestCollision_Cubes(player->ScaleWorldModel(), door->ScaleWorldModel()))
    // {
    //     if (player->color != door->color)
    //     {
    //         std::cout << "debug: hit\n";
    //         // Reverse the move
    //         camera.UpdateCamera2D(mouseDelta, -dm);
    //         pos -= 32.f * forward;
    //     }
    // }

    // Check for collision with the orb
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
    player = new Player(cc);
    player->position = glm::vec3(-64, 0, 0);
    player->absoluteScale = glm::vec3(16, 16, 5);
    player->color = glm::vec4(0, 1, 0, 1);
    player->drawMode = GL_LINE_LOOP;
        
    // Right face:
    Scene_Node* right = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1), glm::vec3(128, 0, 0));
    right->relativeModel = Model;
    right->absoluteScale = glm::vec3(000, 256, 256); 
    room->AddChild(right);
    
    // Left face:
    Scene_Node* left = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1), glm::vec3(-128, 0, 0));
    left->relativeModel = Model;
    left->absoluteScale = glm::vec3(000, 256, 256); 
    room->AddChild(left);

    // Roof face:
    Scene_Node* roof = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1), glm::vec3(0, 256, 0));
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
    Model = glm::translate(glm::mat4(1), glm::vec3(0, 0, -128));
    front->relativeModel = Model;
    front->absoluteScale = glm::vec3(256, 256, 000);
    front->color = glm::vec4(21.f/255, 5.f/255, 42.f/255, 1.f);
    room->AddChild(front);

    // Back face:
    Scene_Node* back = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1), glm::vec3(0, 0, 128));
    back->relativeModel = Model;
    back->absoluteScale = glm::vec3(256, 256, 000);
    back->color = glm::vec4(21.f/255, 5.f/255, 42.f/255, 1.f);
    room->AddChild(back);

    // Door:
    door = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1), glm::vec3(128, 0, 0));
    door->relativeModel = Model;
    door->absoluteScale = glm::vec3(1, 128, 64);
    door->color = glm::vec4(1.f, 0.f, 0.f, 1.f);
    room->AddChild(door);
    
    // Orb:
    orb = new Scene_Node(cc);
    // Scene_Node* orb = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1), glm::vec3(0, 8, -64));
    Model = glm::rotate(Model, glm::radians(45.f), glm::vec3(0, 1, 0));
    Model = glm::rotate(Model, glm::radians(45.f), glm::vec3(1, 0, 0));
    orb->relativeModel = Model;
    orb->absoluteScale = glm::vec3(8);
    orb->color = glm::vec4(1, 0, 0, 1);
    room->AddChild(orb);

    // HUD:
    hud = new Scene_Node;
    hud->relativeModel = glm::translate(glm::mat4(1), glm::vec3(0, -0.9f, 0));

    Scene_Node* primaryColor = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1), glm::vec3(0.75f, 0, 0));
    primaryColor->relativeModel = Model;
    primaryColor->color = player->color;
    hud->AddChild(primaryColor);

    Scene_Node* secondaryColor = new Scene_Node(cc);
    Model = glm::translate(glm::mat4(1), glm::vec3(0.85f, 0, 0));
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
    // Get the minimum (x, y, z) and the maximum (x, y, z) for both shapes.
    // If the ranges intersect in all three axes,
    // then the shapes collide.
    
    glm::vec3 A_min(1000), A_max(-1000);
    glm::vec3 B_min(1000), B_max(-1000);

    glm::vec4 cubeVertices[8] = {
        glm::vec4(-0.5f,  0.5f, -0.5f, 1),
        glm::vec4(-0.5f,  0.5f,  0.5f, 1),
        glm::vec4( 0.5f,  0.5f,  0.5f, 1),
        glm::vec4( 0.5f,  0.5f, -0.5f, 1),
        glm::vec4(-0.5f, -0.5f, -0.5f, 1),
        glm::vec4(-0.5f, -0.5f,  0.5f, 1),
        glm::vec4( 0.5f, -0.5f,  0.5f, 1),
        glm::vec4( 0.5f, -0.5f, -0.5f, 1),
    };
    glm::vec4 transformed(0);
    
    for (GLubyte i = 0; i < 8; i++)
    {
        // A:
        transformed = objectA * cubeVertices[i];

        A_min[0] = transformed[0] < A_min[0] ? transformed[0] : A_min[0];
        A_min[1] = transformed[1] < A_min[1] ? transformed[1] : A_min[1];
        A_min[2] = transformed[2] < A_min[2] ? transformed[2] : A_min[2];
        
        A_max[0] = transformed[0] > A_max[0] ? transformed[0] : A_max[0];
        A_max[1] = transformed[1] > A_max[1] ? transformed[1] : A_max[1];
        A_max[2] = transformed[2] > A_max[2] ? transformed[2] : A_max[2];

        // B:
        transformed = objectB * cubeVertices[i];

        B_min[0] = transformed[0] < B_min[0] ? transformed[0] : B_min[0];
        B_min[1] = transformed[1] < B_min[1] ? transformed[1] : B_min[1];
        B_min[2] = transformed[2] < B_min[2] ? transformed[2] : B_min[2];

        B_max[0] = transformed[0] > B_max[0] ? transformed[0] : B_max[0];
        B_max[1] = transformed[1] > B_max[1] ? transformed[1] : B_max[1];
        B_max[2] = transformed[2] > B_max[2] ? transformed[2] : B_max[2];
    }
    
    return (
        A_min[0] <= B_max[0] && B_min[0] <= A_max[0] &&
        A_min[1] <= B_max[1] && B_min[1] <= A_max[1] &&
        A_min[2] <= B_max[2] && B_min[2] <= A_max[2] 
    );
}

// ====================================================================================================
// Is called whenever a key is pressed/released via GLFW
void Scene::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (action == GLFW_PRESS)
    {
        switch (key)
        {
        case GLFW_KEY_W:
            movementP[2] = 1;
            break;
        case GLFW_KEY_S:
            movementN[2] = 1;
            break;

        case GLFW_KEY_D:
            movementP[0] = 1;
            break;
        case GLFW_KEY_A:
            movementN[0] = 1;
            break;

        case GLFW_KEY_Q:
            movementP[1] = 1;
            break;
        case GLFW_KEY_E:
            movementN[1] = 1;
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
            movementP[2] = 0;
            break;
        case GLFW_KEY_S:
            movementN[2] = 0;
            break;

        case GLFW_KEY_D:
            movementP[0] = 0;
            break;
        case GLFW_KEY_A:
            movementN[0] = 0;
            break;

        case GLFW_KEY_Q:
            movementP[1] = 0;
            break;
        case GLFW_KEY_E:
            movementN[1] = 0;
            break;

        default:
            break;
        }
    }
}

void Scene::MouseCallback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT || button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
            holdMouse = true;
        else if (action == GLFW_RELEASE)
            holdMouse = false;
    }
}
