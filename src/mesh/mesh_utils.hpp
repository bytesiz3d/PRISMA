#pragma once
#include <cmath>

#include "../font/font.hpp"
#include "mesh.hpp"

namespace Mesh_Utils {
Mesh* WhiteCube();

// GLuint eP(GLuint row, GLuint column, GLuint hRes);
// Mesh* WhiteSphere(GLuint hRes = 32, GLuint vRes = 32);
// Mesh* Orbit();
Mesh* Sphere(GLuint hRes = 32, GLuint vRes = 32);

Mesh* TextMesh(const std::string& text, Font* font);

Mesh* LoadMesh(const std::string& filePath);

// Mesh* ColoredSphere(GLuint hRes = 32, GLuint vRes = 32);
// Mesh* ColoredCube();
}  // namespace Mesh_Utils
