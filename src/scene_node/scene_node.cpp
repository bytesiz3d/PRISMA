#include "scene_node.hpp"

Scene_Node::Scene_Node()
    : mesh(nullptr), texture(nullptr), parent(nullptr), worldModel(glm::mat4(1.f)), drawMode(GL_TRIANGLES)
{
    children = std::vector<Scene_Node*>(0);

    relativeModel = glm::mat4(1.f);
    absoluteScale = glm::vec3(1.f);
    color = glm::vec4(1.f);

    // Hardcoded values taken from the lab
    material.ambient = { 1, 1, 1 };
    material.diffuse = { 0.8, 0.8, 0.8 };
    material.specular = { 1, 1, 1 };
    material.shininess = 20;
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

    // Hardcoded values taken from the lab
    material.ambient = { 1, 1, 1 };
    material.diffuse = { 1, 1, 1 };
    material.specular = { 1, 1, 1 };
    material.shininess = 20;
}

Scene_Node::~Scene_Node()
{
    for (auto& child : children)
        if (child)
            delete child;
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
    glm::mat4 mod = ScaleWorldModel();
    if (!mesh)
        return;
    
    glUseProgram(shaderId);

    // Send M and M_it
    int M = glGetUniformLocation(shaderId, "M");
    int M_it = glGetUniformLocation(shaderId, "M_it");
    int tint = glGetUniformLocation(shaderId, "tint");

    glUniformMatrix4fv(M, 1, false, glm::value_ptr(mod));

    if (glm::determinant(mod) == 0)
        std::cout << "uh oh" << std::endl;

    mod = glm::inverse(mod);
    glUniformMatrix4fv(M_it, 1, true, glm::value_ptr(mod));

    // Send material properties
    int mtl_amb = glGetUniformLocation(shaderId, "material.ambient");
    int mtl_diff = glGetUniformLocation(shaderId, "material.diffuse");
    int mtl_spec = glGetUniformLocation(shaderId, "material.specular");
    int mtl_shine = glGetUniformLocation(shaderId, "material.shininess");

    glUniform3f(mtl_amb, material.ambient.r, material.ambient.g, material.ambient.b);
    glUniform3f(mtl_diff, material.diffuse.r, material.diffuse.g, material.diffuse.b);
    glUniform3f(mtl_spec, material.specular.r, material.specular.g, material.specular.b);
    glUniform1f(mtl_shine, material.shininess);

    // Send color
    glUniform4f(tint, color.r, color.g, color.b, color.a);
    
    if (texture)
        texture->Bind();
    
    mesh->Draw(drawMode);
    
    if (texture)
        texture->Unbind();
}
