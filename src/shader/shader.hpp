#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <glad/glad.h>

namespace Shader
{
    /*
      Loads a file's ASCII content into a string and returns it.
      Returns an empty string if it fails.
    */
    std::string LoadFile(const char* filePath);

    /*
      Loads the vertex and fragment shaders from their two respective paths.
      Links the two shaders and returns the compiled program.
    */
    GLuint LoadShader(const char* vertexPath, const char* fragmentPath);
}
