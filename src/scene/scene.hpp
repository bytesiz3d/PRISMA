#include <json/json.hpp>

// Enumerating object type and mesh
#include "enums.hpp"

namespace Scene
{
    GLFWwindow* window;
    std::map<MESH_TYPE, Mesh*> meshes;

    // Controls:
    double p_mX, p_mY,
           mX,   mY;

    glm::vec3 movementP(0);
    glm::vec3 movementN(0);
    
    glm::vec3 dm(0);
    glm::vec2 mouseDelta(0);

    bool holdMouse = false;

    // Camera:
    Camera camera;
    float cameraAngle = 0.f;
    GLint VP_location;
    GLint texture_sampler_location;
    GLint on_location;

    // TODO: Optimize collision checking
    // Main scene:
    Scene_Node* root;
    std::vector<Scene_Node*> walls;
    std::vector<Scene_Node*> doors;
    std::vector<Scene_Node*> orbs;

    // Player and HUD:
    Player* player;
    Scene_Node* hud;

    // Utility functions:
    void InitScene(const std::string& scenePath);
    void ParseScene(Scene_Node* parent, const nlohmann::json& data);

    void UpdateData();
    void DrawScene(Scene_Node* scene, GLuint shaderId);

    bool Collide(Scene_Node* objectA, Scene_Node* objectB);
    void ProcessCollision();

    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
    void MouseCallback(GLFWwindow* window, int button, int action, int mods);

    void DeleteAllPointers();
    
};
