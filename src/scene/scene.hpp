#ifndef SCENE_HPP
#define SCENE_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <unordered_map>
#include <json/json.hpp>

// Enumerating object type and mesh
#include "enums.hpp"
#include "../lamp/Lamp.h"
#include  "../player/player.hpp"
#include "../camera/camera.hpp"

struct Light {
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  glm::vec3 direction;
  glm::vec3 position;
  float attenuation;
};

class Scene {
  static GLFWwindow *window;
  std::map<MESH_TYPE, Mesh *> meshes;
  std::map<MESH_TEXTURE, Texture *> textures;
  std::vector<Light> lights;

  // Camera:
  static Camera camera;
  float cameraAngle = 0.f;

  // TODO: Optimize collision checking
  // Main scene:
  Scene_Node *root;
  std::unordered_map<OBJECT_TYPE, std::vector<Scene_Node *> > objects;

  std::vector<glm::vec3> levelRooms;

  // Player and HUD:
  Player *player;
  Scene_Node *hud;
  Lamp *lamp;

  glm::vec3 getLampPosition(glm::vec3 playerPos);

  void ParseScene(Scene_Node *parent, const nlohmann::json &data, bool isRoot = false);

  bool Collide(Scene_Node *objectA, Scene_Node *objectB);

  bool DoorCollide(Scene_Node *objectA, Scene_Node *objectB);

  void ProcessCollision(const glm::vec2 &cameraMovement, const glm::vec2 &playerMovement);

public:
  Scene(const std::string &scenePath);
  Scene() = default;

  void InitScene(const std::string &scenePath);

  void set_meshes(const std::map<MESH_TYPE, Mesh *> &meshes) {
    this->meshes = meshes;
  }

  void set_textures(const std::map<MESH_TEXTURE, Texture *> &textures) {
    this->textures = textures;
  }

  [[nodiscard]] static const Camera & get_camera() {
    return camera;
  }

  [[nodiscard]] Scene_Node * get_root() const {
    return root;
  }


  [[nodiscard]] Player * get_player() const {
    return player;
  }

  [[nodiscard]] Scene_Node * get_hud() const {
    return hud;
  }

  void set_hud(Scene_Node * const hud) {
    this->hud = hud;
  }

  [[nodiscard]] Lamp * get_lamp() const {
    return lamp;
  }

  static void updateWindowSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    camera.aspectRatio = static_cast<float>(width) / static_cast<float>(height);
  }

  void UpdateData();

  static void DrawScene(Scene_Node *scene, GLuint shaderId);

  void UploadLights(GLuint shaderID);

  static GLFWwindow *getWindow();

  static void setWindow(GLFWwindow *newWindow);

  ~Scene();
}; // namespace Scene

#endif // SCENE_HPP
