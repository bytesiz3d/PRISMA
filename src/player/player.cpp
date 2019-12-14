#include "player.hpp"

Player::Player()
    : Scene_Node()
{
    yaw = 0;
    yawSens = 0.001f;
    pitch = -M_PI/6; // PI/6
    pitchSens = 0.001f;

    direction = glm::vec3(std::cos(yaw), 0, std::sin(yaw));
    normal = glm::vec3(std::sin(yaw), 0, std::cos(yaw));

    movementSens = 0.3f;
}

Player::Player(Mesh* _mesh,
               Scene_Node* _parent,
               glm::mat4 _relativeModel,
               glm::vec3 _absoluteScale,
               glm::vec4 _color)
    : Scene_Node(_mesh, _parent, _relativeModel, _absoluteScale, _color)
{
    yaw = 0;
    yawSens = 0.001f;
    pitch = -M_PI/6;
    pitchSens = 0.001f;

    direction = glm::vec3(std::cos(yaw), 0, std::sin(yaw));
    normal = glm::vec3(std::sin(yaw), 0, std::cos(yaw));

    movementSens = 0.3f;
}


void Player::UpdatePlayer(glm::vec2 mouseDelta, glm::vec3 movement)
{
    yaw += mouseDelta[0] * yawSens;
    pitch += -mouseDelta[1] * pitchSens;
    pitch = glm::min(0.f, glm::max(pitch, (float) -M_PI_2));

    direction = glm::vec3(std::cos(yaw), 0, std::sin(yaw));
    normal = glm::vec3(-std::sin(yaw), 0, std::cos(yaw));
        
    movement *= movementSens;

    position += direction * movement[2];
    position += normal * movement[0];
    // TODO: Add jumping

    // Translate to new position and adjust orientation
    relativeModel = glm::translate(glm::mat4(1), position);
    relativeModel = glm::rotate(relativeModel, (float)M_PI_2 - yaw, glm::vec3(0, 1, 0));
}

