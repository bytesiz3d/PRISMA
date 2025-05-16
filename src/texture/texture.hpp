#pragma once

#include <string>

// GLAD
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Texture {
  GLuint textureID;
  std::string filePath;
  GLubyte* localBuffer;
  GLenum inputFormat;

 public:
  int width, height, bpp;
  Texture(const std::string& path, GLenum format = GL_RGBA);
  Texture(GLubyte color = 0xFF);
  ~Texture();

  void Bind(GLuint slot = 0) const;
  void Unbind() const;
};
