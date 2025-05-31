#pragma once

#include <memory>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <vector>

#include "../mesh/mesh.hpp"
#include "../texture/texture.hpp"

struct Material {
  glm::vec3 ambient;
  glm::vec3 diffuse;
  glm::vec3 specular;
  float shininess;
};

class Scene_Node {
 private:
 public:
  std::shared_ptr<Mesh> mesh;
  Texture* texture;
  Material material;

  Scene_Node* parent;
  std::vector<Scene_Node*> children;

  glm::mat4 relativeModel;
  glm::mat4 worldModel;
  glm::vec3 absoluteScale;

  glm::vec4 color;

  GLenum drawMode;

  Scene_Node();
  Scene_Node(std::shared_ptr<Mesh> _mesh,
             Scene_Node* _parent = nullptr,
             glm::mat4 _relativeModel = glm::mat4(1.f),
             glm::vec3 _absoluteScale = glm::vec3(1.f),
             glm::vec4 _color = glm::vec4(1.f));
  ~Scene_Node();

  glm::mat4 ScaleWorldModel() const;
  void AddChild(Scene_Node* child);
  void Draw(GLuint shaderId);
};
