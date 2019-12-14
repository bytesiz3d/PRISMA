#pragma once

#include "../scene_node/scene_node.hpp"

class Player
    : public Scene_Node
{
private:
    float yaw;
    float yawSens;
    float pitchSens;
    float movementSens;
public:
    float pitch;
    glm::vec3 direction;
    glm::vec3 position;
    glm::vec3 normal;
    
    Player();
    Player(Mesh* _mesh,
           Scene_Node* _parent = nullptr,
           glm::mat4 _relativeModel = glm::mat4(1.f),
           glm::vec3 _absoluteScale = glm::vec3(1.f),
           glm::vec4 _color = glm::vec4(1.f)
    );

    void UpdatePlayer(glm::vec2 mouseDelta, glm::vec3 movement);
};