#pragma once
#include <cmath>
#include "mesh.hpp"
#include "../font/font.hpp"

namespace Mesh_Utils
{
    GLuint eP(GLuint row, GLuint column, GLuint hRes);
    
    Mesh* ColoredSphere(GLuint hRes = 32, GLuint vRes = 32);
    Mesh* ColoredCube();
    Mesh* WhiteCube();
    Mesh* Orbit();
    Mesh* OBJMesh(const std::string& filePath);
    Mesh* TextMesh(const std::string& text, Font* font);
}
