#include <json/json.hpp>

// Enumerating object type and mesh
#include "enums.hpp"
#include "../lamp/Lamp.h"

struct Light
{
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 direction;
    glm::vec3 position;
    float attenuation;
} light;

namespace Scene
{
    GLFWwindow* window;
    std::map<MESH_TYPE, Mesh*> meshes;
    std::map<MESH_TEXTURE, Texture*> textures;
    std::vector<Light> lights;

    // Controls:
    double p_mX, p_mY,
           mX,   mY;

    glm::vec3 movementP(0);
    glm::vec3 movementN(0);
    
    glm::vec3 dm(0);
    glm::vec2 mouseDelta(0);

    bool holdMouse = false;
    bool swapped = false;

    // Camera:
    Camera camera;
    float cameraAngle = 0.f;
    GLint VP_location;
    GLint texture_sampler_location;
    GLint cam_pos_location;

    // TODO: Optimize collision checking
    // Main scene:
    Scene_Node* root;
    std::vector<Scene_Node*> walls;
    std::vector<Scene_Node*> doors;
    std::vector<Scene_Node*> orbs;

    float levelTwoRoomsX[9]={0,256,0,-256,-256,0,0,256,512};
    float levelTwoRoomsZ[9]={0,0,256,256,512,-256,-512,-512,-512};

    float levelOneRoomsX[7]={0,256,256,512,256,512,0};
    float levelOneRoomsZ[7]={0,0,256,256,-256,-256,-256};

    // Player and HUD:
    Player* player;
    Scene_Node* hud;
    Lamp* lamp;

    // Utility functions:
    void InitScene(const std::string& scenePath);
    void ParseScene(Scene_Node* parent, const nlohmann::json& data);

    glm::vec3 getNearestRoomPositionLv2(glm::vec3 playerPos);
    glm::vec3 getNearestRoomPositionLv1(glm::vec3 playerPos);

    void UpdateData();
    void DrawScene(Scene_Node* scene, GLuint shaderId);
    void UploadLights(GLuint shaderID);

    bool Collide(Scene_Node* objectA, Scene_Node* objectB);
    void ProcessCollision();

    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
    void MouseCallback(GLFWwindow* window, int button, int action, int mods);

    void DeleteAllPointers();
    
};
