// STB_IMAGE
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "texture.hpp"

// Creates a colored texture
Texture::Texture(GLubyte color)
    : textureID(0), filePath(""), inputFormat(GL_RGBA), localBuffer(nullptr), width(256), height(256), bpp(0)
{
    localBuffer = new GLubyte[width * height * 4];
    memset(localBuffer, color, width * height * 4);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Sampling options:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
 
    // Upload the texture
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, inputFormat,
                 width, height, 0, inputFormat,
                 GL_UNSIGNED_BYTE, localBuffer);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::Texture(const std::string& path, GLenum format)
    : textureID(0), filePath(path), inputFormat(format), localBuffer(nullptr), width(0), height(0), bpp(0)
{
    // OpenGL coordinates start at bottom left
    stbi_set_flip_vertically_on_load(1);

    // Load the buffer
    localBuffer = stbi_load(path.c_str(), &width, &height, &bpp, 0);

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Sampling options:
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

 
    // Upload the texture
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, inputFormat,
                 width, height, 0, inputFormat,
                 GL_UNSIGNED_BYTE, localBuffer);
    glGenerateMipmap(GL_TEXTURE_2D);

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
