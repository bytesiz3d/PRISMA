#pragma once

#include <vector>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../mesh/mesh.hpp"

class Scene_Node
{
private:
public:
    Mesh* mesh;
    Scene_Node* parent;
    std::vector<Scene_Node*> children;

    glm::mat4 relativeModel;
    glm::mat4 worldModel;
    glm::vec3 absoluteScale;

    glm::vec4 color;

    GLenum drawMode;
    
    Scene_Node();
    Scene_Node(Mesh* _mesh,
               Scene_Node* _parent = nullptr,
               glm::mat4 _relativeModel = glm::mat4(1.f),
               glm::vec3 _absoluteScale = glm::vec3(1.f),
               glm::vec4 _color = glm::vec4(1.f));
    ~Scene_Node();

    glm::mat4 ScaleWorldModel();
    void AddChild(Scene_Node* child);
    void Draw(GLuint shaderId);
};
