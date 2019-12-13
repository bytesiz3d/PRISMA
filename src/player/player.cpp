#include "player.hpp"

Player::Player()
    : Scene_Node()
{
    yaw = 0;
    yawSens = 0.001;

    direction = glm::vec3(std::cos(yaw), 0, std::sin(yaw));
    normal = glm::vec3(std::sin(yaw), 0, std::cos(yaw));

    movementSens = 0.3;
}

Player::Player(Mesh* _mesh,
               Scene_Node* _parent,
               glm::mat4 _relativeModel,
               glm::vec3 _absoluteScale,
               glm::vec4 _color)
    : Scene_Node(_mesh, _parent, _relativeModel, _absoluteScale, _color)
{
    yaw = 0;
    yawSens = 0.001;

    direction = glm::vec3(std::cos(yaw), 0, std::sin(yaw));
    normal = glm::vec3(std::sin(yaw), 0, std::cos(yaw));

    movementSens = 0.3;
}


void Player::UpdatePlayer(float _yaw, glm::vec3 movement)
{
    yaw += _yaw * yawSens;
    direction = glm::vec3(std::cos(yaw), 0, std::sin(yaw));
    normal = glm::vec3(-std::sin(yaw), 0, std::cos(yaw));
        
    movement *= movementSens;

    position += direction * movement[2];
    position += normal * movement[0];
    // TODO: Add jumping

    // Translate to new position and adjust orientation
    relativeModel = glm::translate(glm::mat4(1), position);
    relativeModel = glm::rotate(relativeModel, -yaw, glm::vec3(0, 1, 0));
}

