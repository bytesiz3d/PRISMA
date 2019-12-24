
#include "Lamp.h"

Lamp::Lamp()
        : Scene_Node()
{
    orientation = glm::vec3(0.f);
    yaw = 0;
    yawSens = 0.001f;
    pitch = -0.523598776f; // PI/6
    pitchSens = 0.001f;

    direction = glm::vec3(std::cos(yaw), 0, std::sin(yaw));
    normal = glm::vec3(std::sin(yaw), 0, std::cos(yaw));

    movementSens = 0.3f;
}

Lamp::Lamp(Mesh* _mesh,
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

    direction = glm::vec3(std::cos(yaw), 0, std::sin(yaw));
    normal = glm::vec3(std::sin(yaw), 0, std::cos(yaw));

    movementSens = 0.3f;
}

void Lamp::UpdateLamp()
{

    relativeModel = glm::translate(glm::mat4(1), position);

}




