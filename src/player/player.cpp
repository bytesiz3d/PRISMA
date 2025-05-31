#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "player.hpp"
void Player::init() {
  orientation = glm::vec3(0.f);
  yaw = 0;
  pitch = glm::radians(-30.f);
}

Player::Player() {
  init();
}

Player::Player(Mesh* _mesh,
               Scene_Node* _parent,
               glm::mat4 _relativeModel,
               glm::vec3 _absoluteScale,
               glm::vec4 _color)
  : Scene_Node(_mesh, _parent, _relativeModel, _absoluteScale, _color) {
  init();
}

void Player::UpdatePlayer(glm::vec2 mouseDelta, glm::vec2 movement, float deltaTime) {
  constexpr float YAW_SENSITIVITY = 500;  // Sensitivity for yaw rotation
  constexpr float PITCH_SENSITIVITY = 500;  // Sensitivity for pitch rotation
  constexpr float MOVEMENT_SENSITIVITY = 50;  // Sensitivity for movement

  yaw += mouseDelta.x * YAW_SENSITIVITY * deltaTime;
  pitch += -mouseDelta.y * PITCH_SENSITIVITY * deltaTime;
  pitch = glm::clamp(pitch, glm::radians(-89.9f), glm::radians(10.0f));

  direction = glm::vec3(std::cos(yaw), 0, std::sin(yaw));
  normal = glm::vec3(-std::sin(yaw), 0, std::cos(yaw));

  movement *= MOVEMENT_SENSITIVITY * deltaTime;

  position += direction * movement.y;
  position += normal * movement.x;
  // TODO: Add jumping

  // Translate to new position and adjust orientation
  relativeModel = glm::translate(glm::mat4(1), position);
  relativeModel = glm::rotate(relativeModel, -yaw, glm::vec3(0, 1, 0));

  // Order was determined **empirically**
  relativeModel = glm::rotate(relativeModel, orientation[1], glm::vec3(0, 1, 0));
  relativeModel = glm::rotate(relativeModel, orientation[2], glm::vec3(0, 0, 1));
  relativeModel = glm::rotate(relativeModel, orientation[0], glm::vec3(1, 0, 0));
}

