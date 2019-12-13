#pragma once

#include <string>

// GLAD
#include <glad/glad.h>

class Texture
{
    GLuint textureID;
    std::string filePath;
    GLubyte* localBuffer;

public:
    int width, height, bpp;
    Texture(const std::string& path);
    ~Texture();

    void Bind(GLuint slot = 0) const;
    void Unbind() const;
};
