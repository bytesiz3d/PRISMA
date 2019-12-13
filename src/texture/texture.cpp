// STB_IMAGE
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "texture.hpp"

Texture::Texture(const std::string& path)
    : textureID(0), filePath(path), localBuffer(nullptr), width(0), height(0), bpp(0)
{
    // OpenGL coordinates start at bottom left
    stbi_set_flip_vertically_on_load(1);
    // Load the buffer
    localBuffer = stbi_load(path.c_str(), &width, &height, &bpp, 4);

    glGenBuffers(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Sampling options:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Upload the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8,
                 width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, localBuffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    // Free the buffer
    if (localBuffer)
        stbi_image_free(localBuffer);
}

Texture::~Texture()
{
    glDeleteTextures(1, &textureID);
}

void Texture::Bind(GLuint slot) const
{
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

void Texture::Unbind() const
{
    glBindTexture(GL_TEXTURE_2D, 0);
}
