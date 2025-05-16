#pragma once
#include <iostream>
#include <map>
#include <string>
#include <vector>

// GLAD
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// GLM
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>

struct VertexDescriptor {
  GLuint attributeLocation;
  std::string buffer;
  GLint size;
  GLenum type;
  GLboolean normalized;
  GLsizei stride;
  void* offset;
};

class Mesh {
 private:
  std::vector<VertexDescriptor> descriptors;
  std::map<std::string, GLuint> VBOs;

  GLuint EBO;
  GLuint VAO;

  GLsizei elementCount;
  GLenum elementType;

 public:
  // Axis-aligned bounding box info:
  glm::vec3 AABB_min, AABB_max;

  Mesh(VertexDescriptor* inDescriptors = nullptr, int n = 3);
  ~Mesh();

  void SetBufferData(std::string bufferName, GLsizeiptr size, const void* bufferData, GLenum usage);
  void SetElementsData(GLsizeiptr size, const void* bufferData, GLenum usage, GLsizei count, GLenum type);
  void Draw(GLenum mode = GL_TRIANGLES);
};
