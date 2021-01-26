#pragma once
#include "../scene_node/scene_node.hpp"

class Orb
  : public Scene_Node {
 public:
  bool lastState, currentState;

  Orb();
  ~Orb();
  Orb(Mesh* _mesh,
      Scene_Node* _parent = nullptr,
      glm::mat4 _relativeModel = glm::mat4(1.f),
      glm::vec3 _absoluteScale = glm::vec3(1.f),
      glm::vec4 _color = glm::vec4(1.f));
};
