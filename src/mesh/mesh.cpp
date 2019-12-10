#include "mesh.hpp"

Mesh::Mesh()
{
    int n = 2;
    VertexDescriptor inDescriptors[] = {
        {
            0,
            "positions",
            3,
            GL_FLOAT,
            false,
            0,
            0
        },
        {
            1,
            "colors",
            4,
            GL_UNSIGNED_BYTE,
            true,
            0,
            0
        }
    };

    for (GLuint i = 0; i < n; i++)
        descriptors.push_back(inDescriptors[i]);

    for (auto& desc : descriptors)
    {
        glGenBuffers(1, &VBOs[desc.buffer]);
    }

    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    for (auto& descriptor : descriptors)
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[descriptor.buffer]);

        glVertexAttribPointer(descriptor.attributeLocation,
                              descriptor.size,
                              descriptor.type,
                              descriptor.normalized,
                              descriptor.stride,
                              descriptor.offset);

        glEnableVertexAttribArray(descriptor.attributeLocation);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindVertexArray(0);
}

Mesh::~Mesh()
{
    for (auto& desc : descriptors)
    {
        glDeleteBuffers(1, &VBOs[desc.buffer]);
    }
    VBOs.clear();

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &EBO);
}

Mesh::Mesh(VertexDescriptor* inDescriptors, int n)
{
    for (GLuint i = 0; i < n; i++)
        descriptors.push_back(inDescriptors[i]);

    for (auto& desc : descriptors)
    {
        glGenBuffers(1, &VBOs[desc.buffer]);
    }

    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);

    for (auto& descriptor : descriptors)
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[descriptor.buffer]);

        glVertexAttribPointer(descriptor.attributeLocation,
                              descriptor.size,
                              descriptor.type,
                              descriptor.normalized,
                              descriptor.stride,
                              descriptor.offset);

        glEnableVertexAttribArray(descriptor.attributeLocation);
    }
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBindVertexArray(0);
}

void Mesh::SetBufferData(std::string bufferName, GLsizeiptr size, const void* bufferData, GLenum usage)
{
    if (VBOs[bufferName] != 0)
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[bufferName]);
        glBufferData(GL_ARRAY_BUFFER, size, bufferData, usage);
    }
    else
    {
        std::cerr << bufferName << " is not found in the buffers list\n";
    }
}

void Mesh::SetElementsData(GLsizeiptr size, const void* bufferData, GLenum usage, GLsizei count, GLenum type)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, bufferData, usage);

    elementCount = count;
    elementType = type;
}

void Mesh::Draw(GLenum mode)
{
    glBindVertexArray(VAO);
    glDrawElements(mode, elementCount, elementType, 0);
    glBindVertexArray(0);
}
