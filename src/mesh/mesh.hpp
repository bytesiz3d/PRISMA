#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <glad/glad.h>

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
    Mesh(VertexDescriptor* inDescriptors, int n);
    ~Mesh();
	
    void setBufferData(std::string bufferName, GLsizeiptr size, const void* bufferData, GLenum usage);
    void setElementsData(GLsizeiptr size, const void* bufferData, GLenum usage, GLsizei count, GLenum type);
    void draw(GLenum mode = GL_TRIANGLES);
};
