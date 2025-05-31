#include "scene_node.hpp"

#include <memory>

Scene_Node::Scene_Node()
  : mesh(nullptr), texture(nullptr), parent(nullptr), worldModel(glm::mat4(1.f)), drawMode(GL_TRIANGLES) {

  relativeModel = glm::mat4(1.f);
  absoluteScale = glm::vec3(1.f);
  color = glm::vec4(1.f);

  // Hardcoded values taken from the lab
  material.ambient = {1, 1, 1};
  material.diffuse = {0.8, 0.8, 0.8};
  material.specular = {1, 1, 1};
  material.shininess = 20;
}

Scene_Node::Scene_Node(const std::shared_ptr<Mesh> _mesh,
                       Scene_Node* _parent,
                       glm::mat4 _relativeModel,
                       glm::vec3 _absoluteScale,
                       glm::vec4 _color)
  : mesh(nullptr), texture(nullptr), parent(nullptr), worldModel(glm::mat4(1.f)), drawMode(GL_TRIANGLES) {
  mesh = _mesh;

  parent = _parent;

  relativeModel = _relativeModel;
  absoluteScale = _absoluteScale;
  color = _color;

  // Hardcoded values taken from the lab
  material.ambient = {1, 1, 1};
  material.diffuse = {1, 1, 1};
  material.specular = {1, 1, 1};
  material.shininess = 20;
}

void Scene_Node::AddChild(std::unique_ptr<Scene_Node>&& child) {
  child->parent = this;
  children.push_back(std::move(child));
}

glm::mat4 Scene_Node::ScaleWorldModel() const {
  auto model = worldModel;
  if (parent) {
    model *= parent->ScaleWorldModel();
  }
  model *= relativeModel;
  return glm::scale(model, absoluteScale);
}

void Scene_Node::Draw(const Shader &shader) const {
  glm::mat4 mod = ScaleWorldModel();

  if (glm::determinant(mod) == 0) {
    // handle error
    std::cout << "uh oh" << std::endl;
    return;
  }

  shader.setUniform("M", mod);
  mod = glm::inverse(mod);
  shader.setUniform("M_it", mod);

  shader.setUniform("material.ambient", material.ambient);
  shader.setUniform("material.diffuse", material.diffuse);
  shader.setUniform("material.specular", material.specular);
  shader.setUniform("material.shininess", material.shininess);

  // Send color
  shader.setUniform("tint", color);

  if (mesh) {
    if (texture)
      texture->Bind();

    mesh->Draw(drawMode);

    if (texture)
      texture->Unbind();
  }
}
