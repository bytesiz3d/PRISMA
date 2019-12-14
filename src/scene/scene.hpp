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
    
    glm::vec3 dm(0);
    glm::vec2 mouseDelta(0);

    bool holdMouse = false;

    // Utility functions:
    void UpdateData();
    void InitScene(Mesh* cc);
    void DrawScene(Scene_Node* scene, GLuint shaderId);

    bool Collide(Scene_Node* objectA, Scene_Node* objectB);
    void ProcessCollision();

    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
    void MouseCallback(GLFWwindow* window, int button, int action, int mods);

    void DeleteAllPointers();

    // Camera:
    Camera camera;
    float cameraAngle = 0.f;
    GLint VP_location;
    GLint texture_sampler_location;

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
