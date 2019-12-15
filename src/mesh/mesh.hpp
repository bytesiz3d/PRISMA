#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>

// GLAD
#include <glad/glad.h>

// GLM
//#include <glm/vec3.hpp>

struct VertexDescriptor
{
    GLuint attributeLocation;
    std::string buffer;
    GLint size;
    GLenum type;
    GLboolean normalized;
    GLsizei stride;
    void* offset;
};

class Mesh
{
private:
    std::vector<VertexDescriptor> descriptors;
    std::map<std::string, GLuint> VBOs;
    
    GLuint EBO;
    GLuint VAO;

    GLsizei elementCount;
    GLenum elementType;

public:
    // Axis-aligned bounding box info:
    float AABB_min[3], AABB_max[3];

    Mesh(VertexDescriptor* inDescriptors = nullptr, int n = 3);
    ~Mesh();
	
    void SetBufferData(std::string bufferName, GLsizeiptr size, const void* bufferData, GLenum usage);
    void SetElementsData(GLsizeiptr size, const void* bufferData, GLenum usage, GLsizei count, GLenum type);
    void Draw(GLenum mode = GL_TRIANGLES);
};

