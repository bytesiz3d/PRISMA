#include "mesh.hpp"

// positions, colors, texture coordinates
VertexDescriptor vd_pct[3] = {
  {0,
   "positions",
   3,
   GL_FLOAT,
   false,
   0,
   0},
  {1,
   "normals",
   3,
   GL_FLOAT,
   false,
   0,
   0},
  {2,
   "texcoords",
   2,
   GL_FLOAT,
   false,
   0,
   0},
};

Mesh::Mesh(VertexDescriptor* inDescriptors, int n)
  : elementCount(0), elementType(0) {
  AABB_min = glm::vec3(1000.f);
  AABB_max = glm::vec3(-1000.f);

  if (!inDescriptors) {
    inDescriptors = vd_pct;
    n = sizeof(vd_pct) / sizeof(VertexDescriptor);
  }

  for (GLuint i = 0; i < n; i++)
    descriptors.push_back(inDescriptors[i]);

  for (auto& desc: descriptors) {
    glGenBuffers(1, &VBOs[desc.buffer]);
  }

  glGenBuffers(1, &EBO);
  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);

  for (auto& descriptor: descriptors) {
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

Mesh::~Mesh() {
  for (auto& desc: descriptors) {
    glDeleteBuffers(1, &VBOs[desc.buffer]);
  }
  VBOs.clear();

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &EBO);
}

void Mesh::SetBufferData(std::string bufferName, GLsizeiptr size, const void* bufferData, GLenum usage) {
  if (VBOs[bufferName] != 0) {
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[bufferName]);
    glBufferData(GL_ARRAY_BUFFER, size, bufferData, usage);
  }
  else {
    std::cerr << bufferName << " is not found in the buffers list\n";
  }
}

void Mesh::SetElementsData(GLsizeiptr size, const void* bufferData, GLenum usage, GLsizei count, GLenum type) {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, bufferData, usage);

  elementCount = count;
  elementType = type;
}

void Mesh::Draw(GLenum mode) {
  glBindVertexArray(VAO);
  glDrawElements(mode, elementCount, elementType, 0);
  glBindVertexArray(0);
}
