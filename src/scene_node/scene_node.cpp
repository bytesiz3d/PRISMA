#include "scene_node.hpp"

Scene_Node::Scene_Node()
    : mesh(nullptr), texture(nullptr), parent(nullptr), worldModel(glm::mat4(1.f)), drawMode(GL_TRIANGLES)
{
    children = std::vector<Scene_Node*>(0);

    relativeModel = glm::mat4(1.f);
    absoluteScale = glm::vec3(1.f);
    color = glm::vec4(1.f);
}

Scene_Node::Scene_Node(Mesh* _mesh,
                       Scene_Node* _parent,
                       glm::mat4 _relativeModel,
                       glm::vec3 _absoluteScale,
                       glm::vec4 _color)
    : mesh(nullptr), texture(nullptr), parent(nullptr), worldModel(glm::mat4(1.f)), drawMode(GL_TRIANGLES)
{
    mesh = _mesh;
    
    parent = _parent;
    children = std::vector<Scene_Node*>(0);

    relativeModel = _relativeModel;
    absoluteScale = _absoluteScale;
    color = _color;
}

Scene_Node::~Scene_Node()
{
    for (auto& child : children)
    {
        if (child)
            delete child;
        child = nullptr;
    }
}

void Scene_Node::AddChild(Scene_Node* child)
{
    children.push_back(child);
    child->parent = this;
}

glm::mat4 Scene_Node::ScaleWorldModel()
{
    worldModel = parent ? parent->worldModel : glm::mat4(1.f); 
    worldModel *= relativeModel;
    return glm::scale(worldModel, absoluteScale);
}

void Scene_Node::Draw(GLuint shaderId)
{
    glUseProgram(shaderId);
    int M_location = glGetUniformLocation(shaderId, "M");
    int tint_location = glGetUniformLocation(shaderId, "tint");

    glm::mat4 scaledWorldModel = ScaleWorldModel();
    glUniformMatrix4fv(M_location, 1, false, glm::value_ptr(scaledWorldModel));

    glm::mat4 tint(0.f);
    tint[3] = color;
    glUniformMatrix4fv(tint_location, 1, false, glm::value_ptr(tint));

    if (mesh)
    {
        if (texture)
            texture->Bind();

        mesh->Draw(drawMode);
        
        if (texture)
            texture->Unbind();
    }
}
