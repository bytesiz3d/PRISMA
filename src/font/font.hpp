#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <array>

// GLAD
#include <glad/glad.h>

// GLM
#include <stb/stb_truetype.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

struct GlyphInfo {
    std::array<glm::vec3, 4> positions = { glm::vec3(0.f) };
    std::array<glm::vec2, 4> uvs = { glm::vec2(0.f) };
    float offsetX = 0.f;
    float offsetY = 0.f;
};

class Font {
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
    
    GlyphInfo GetGlyphInfo(GLubyte c, float offsetX, float offsetY) const;

    void Bind(GLuint slot = 0) const;
    void Unbind() const;
};
