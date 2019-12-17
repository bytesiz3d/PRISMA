#include "orb.hpp"

Orb::Orb()
    : Scene_Node(), lastState(false), currentState(false)
{}

Orb::~Orb()
{
}

Orb::Orb(Mesh* _mesh,             
         Scene_Node* _parent,     
         glm::mat4 _relativeModel,
         glm::vec3 _absoluteScale,
         glm::vec4 _color)        
    : Scene_Node(_mesh, _parent, _relativeModel, _absoluteScale, _color),
    lastState(false), currentState(false)
{}
