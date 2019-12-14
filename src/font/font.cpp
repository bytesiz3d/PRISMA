// STB_TRUETYPE
#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
//#include <stb/stb_truetype.h>
#include "font.hpp"

Font::Font(const std::string& path, GLuint _size)
    : size(_size), atlasWidth(1024), atlasHeight(1024),
      oversampleX(2), oversampleY(2),
      firstChar(' '), charCount('~' - ' '),
      textureID(0), filePath(path)
{
    // Open the file
    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file " << filePath << std::endl;
        return; // ?
    }

    // Read file contents
    const auto f_size = file.tellg();
    file.seekg(0, std::ios::beg);
    auto fontData = std::vector<GLubyte>(f_size);
    file.read(reinterpret_cast<char*>(&fontData[0]), f_size);
    file.close();
    
    // Allocate memory
    auto atlasData = std::make_unique<GLubyte[]>(atlasWidth * atlasHeight);
    charInfo = std::make_unique<stbtt_packedchar[]>(charCount);

    stbtt_pack_context context;
    if (!stbtt_PackBegin(&context, atlasData.get(), atlasWidth, atlasHeight, 0, 1, nullptr))
    {
        std::cerr << "Failed to initialize font" << std::endl;
        return; // ?
    }

    // Pack font
    stbtt_PackSetOversampling(&context, oversampleX, oversampleY);
    if (!stbtt_PackFontRange(&context, fontData.data(), 0, size, firstChar, charCount, charInfo.get()))
    {
        std::cerr << "Failed to pack font" << std::endl;
        return; // !?!?!?!??!????
    }

    stbtt_PackEnd(&context);

    // Generate the font texture
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Sampling options:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Upload the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB,
                 atlasWidth, atlasWidth, 0, GL_RED,
                 GL_UNSIGNED_BYTE, atlasData.get());
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

GlyphInfo Font::GetGlyphInfo(GLubyte c, float offsetX, float offsetY)
{
    stbtt_aligned_quad quad;

    stbtt_GetPackedQuad(charInfo.get(), atlasWidth, atlasHeight,
                        c - firstChar, &offsetX, &offsetY, &quad, 1);

    auto xmin = quad.x0;
    auto xmax = quad.x1;
    auto ymin = -quad.y1;
    auto ymax = -quad.y0;

    GlyphInfo info{};
    info.offsetX = offsetX;
    info.offsetY = offsetY;
    info.positions[0] = {xmin, ymin, 0};
    info.positions[1] = {xmin, ymax, 0};
    info.positions[2] = {xmax, ymax, 0};
    info.positions[3] = {xmax, ymin, 0};
    info.uvs[0] = {quad.s0, quad.t1};
    info.uvs[1] = {quad.s0, quad.t0};
    info.uvs[2] = {quad.s1, quad.t0};
    info.uvs[3] = {quad.s1, quad.t1};

    return info;
    
}

Font::~Font()
{
    glDeleteTextures(1, &textureID);
}

void Font::Bind(GLuint slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Font::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
