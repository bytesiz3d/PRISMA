#include "player.hpp"

Player::Player()
    : Scene_Node()
{
    orientation = glm::vec3(0.f);
    yaw = 0;
    yawSens = 0.001f;
    pitch = -0.523598776f; // PI/6
    pitchSens = 0.001f;

    jumped = false;
    v_velocity = 0;

    direction = glm::vec3(std::cos(yaw), 0, std::sin(yaw));
    normal = glm::vec3(std::sin(yaw), 0, std::cos(yaw));

    movementSens = 0.25f;
}

Player::Player(Mesh* _mesh,
               Scene_Node* _parent,
               glm::mat4 _relativeModel,
               glm::vec3 _absoluteScale,
               glm::vec4 _color)
    : Scene_Node(_mesh, _parent, _relativeModel, _absoluteScale, _color)
{
    orientation = glm::vec3(0.f);
    yaw = 0;
    yawSens = 0.001f;
    pitch = -0.523598776f;
    pitchSens = 0.001f;

    jumped = false;
    v_velocity = 0;

    direction = glm::vec3(std::cos(yaw), 0, std::sin(yaw));
    normal = glm::vec3(std::sin(yaw), 0, std::cos(yaw));

    movementSens = 0.2f;
}


void Player::UpdatePlayer(glm::vec2 mouseDelta, glm::vec3 movement)
{
    yaw += mouseDelta[0] * yawSens;
    pitch += -mouseDelta[1] * pitchSens;
    pitch = glm::min(0.f, glm::max(pitch, -1.57079633f)); 

    direction = glm::vec3(std::cos(yaw), 0, std::sin(yaw));
    normal = glm::vec3(-std::sin(yaw), 0, std::cos(yaw));
        
    movement *= movementSens;

    position += direction * movement[2];
    position += normal * movement[0];

    // Update vertical velocity
    if (jumped)
        v_velocity -= movementSens / 2;
    else
    {
        v_velocity = 0;
        jumped = false;
        // position[1] = 0;
    }
    position[1] += v_velocity * movementSens;

    
    // Translate to new position and adjust orientation
    relativeModel = glm::translate(glm::mat4(1), position);
    relativeModel = glm::rotate(relativeModel, -yaw, glm::vec3(0, 1, 0));

    // Order was determined **empirically**
    relativeModel = glm::rotate(relativeModel, orientation[1], glm::vec3(0, 1, 0));
    relativeModel = glm::rotate(relativeModel, orientation[2], glm::vec3(0, 0, 1));
    relativeModel = glm::rotate(relativeModel, orientation[0], glm::vec3(1, 0, 0));
}

