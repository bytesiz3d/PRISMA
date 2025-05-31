#ifndef SCENE_HPP
#define SCENE_HPP

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <span>
#include <unordered_map>
#include <json/json.hpp>

// Enumerating object type and mesh
#include "enums.hpp"
#include "../lamp/Lamp.h"
#include  "../player/player.hpp"
#include "../camera/camera.hpp"
#include "../font/font.hpp"
#include "../shader/shader.hpp"

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
  std::unordered_map<MESH_TYPE, std::shared_ptr<Mesh>> meshes;
  std::map<MESH_TEXTURE, Texture *> textures;
  std::vector<Light> lights;

  // Camera:
  static Camera camera;
  float cameraAngle = 0.f;

  // TODO: Optimize collision checking
  // Main scene:
  std::unique_ptr<Scene_Node> root;
  std::unordered_map<OBJECT_TYPE, std::vector<Scene_Node *> > objects;

  std::vector<glm::vec3> levelRooms;

  // Player and HUD:
  std::unique_ptr<Player> player;
  std::unique_ptr<Scene_Node> hud;
  std::unique_ptr<Lamp> lamp;

  glm::vec3 getLampPosition(glm::vec3 playerPos);

  void ParseScene(Scene_Node *parent, const nlohmann::json &data, bool isRoot = false);

  bool Collide(Scene_Node *objectA, Scene_Node *objectB);

  bool DoorCollide(Scene_Node *objectA, Scene_Node *objectB);

  void ProcessCollision(const glm::vec2 &cameraMovement, const glm::vec2 &playerMovement, float deltaTime);

public:
  Scene(const std::string &scenePath);
  Scene() = default;

  void InitScene(const std::string &scenePath);

  static std::tuple<std::unique_ptr<Scene_Node>, std::span<std::unique_ptr<Scene_Node>>> InitMainMenu(const std::string &levelsPath, const Font &font);

  void set_meshes(const std::unordered_map<MESH_TYPE, std::shared_ptr<Mesh>> &meshes) {
    this->meshes = meshes;
  }

  void set_textures(const std::map<MESH_TEXTURE, Texture *> &textures) {
    this->textures = textures;
  }

  [[nodiscard]] static const Camera & get_camera() {
    return camera;
  }

  [[nodiscard]] const std::unique_ptr<Scene_Node>& get_root() const {
    return root;
  }

  [[nodiscard]] const std::unique_ptr<Player>& get_player() const {
    return player;
  }

  [[nodiscard]] const std::unique_ptr<Scene_Node>& get_hud() const {
    return hud;
  }

  [[nodiscard]] const std::unique_ptr<Lamp>& get_lamp() const {
    return lamp;
  }

  static void updateWindowSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    camera.aspectRatio = static_cast<float>(width) / static_cast<float>(height);
  }

  void UpdateData(float deltaTime);


  static void DrawScene(const Scene_Node& scene, const Shader &shader);

  static std::unique_ptr<Scene_Node> Text(const char* string,
                        const Font& font,
                        glm::vec2 position = glm::vec3(0.f),
                        glm::vec4 color = glm::vec4(1.f),
                        float scale = 1,
                        bool center_x = true);

  void UploadLights(const Shader &shader) const;

  static GLFWwindow *getWindow();

  static void setWindow(GLFWwindow *newWindow);

}; // namespace Scene

#endif // SCENE_HPP
