#pragma once
#include <cmath>
#include "mesh.hpp"
#include "../font/font.hpp"

namespace Mesh_Utils
{
    Mesh* WhiteCube();

    GLuint eP(GLuint row, GLuint column, GLuint hRes);
    Mesh* WhiteSphere(GLuint hRes = 32, GLuint vRes = 32);
    Mesh* Orbit();

    Mesh* OBJMesh(const std::string& filePath);
    Mesh* TextMesh(const std::string& text, Font* font);

    // Mesh* ColoredSphere(GLuint hRes = 32, GLuint vRes = 32);
    // Mesh* ColoredCube();
}
