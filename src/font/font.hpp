#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>

// GLAD
#include <glad/glad.h>

// GLM
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include <stb/stb_truetype.h>

struct GlyphInfo
{
    glm::vec3 positions[4] = { glm::vec3(0.f) };
    glm::vec2 uvs[4] = { glm::vec2(0.f) };
    float offsetX = 0.f;
    float offsetY = 0.f;
};

class Font
{
    GLuint textureID;
    std::string filePath; 
    
    std::unique_ptr<stbtt_packedchar[]> charInfo;

    // Font data:
    GLuint size;
    GLuint atlasWidth;
    GLuint atlasHeight;
    GLuint oversampleX;
    GLuint oversampleY;
    GLuint firstChar;
    GLuint charCount;

public:
    Font(const std::string& path, GLuint _size = 32);
    ~Font();
    
    GlyphInfo GetGlyphInfo(GLubyte c, float offsetX, float offsetY);

    void Bind(GLuint slot = 0) const;
    void Unbind() const;
};
