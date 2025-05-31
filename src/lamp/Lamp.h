#pragma once

#include "../scene_node/scene_node.hpp"

class Lamp
        : public Scene_Node
{
private:
    float yawSens;
    float pitchSens;
    float movementSens;
public:
    float yaw;
    float pitch;
    glm::vec3 direction;
    glm::vec3 position;
    glm::vec3 normal;

    // Rotation angles around the 3 axes:
    glm::vec3 orientation;

    Lamp();
    Lamp(std::shared_ptr<Mesh> _mesh,
           Scene_Node* _parent = nullptr,
           glm::mat4 _relativeModel = glm::mat4(1.f),
           glm::vec3 _absoluteScale = glm::vec3(1.f),
           glm::vec4 _color = glm::vec4(1.f)
    );

    void UpdateLamp(glm::vec3 newPosition);
};
