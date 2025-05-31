#pragma once
#include <cmath>
#include <memory>

#include "../font/font.hpp"
#include "mesh.hpp"

namespace Mesh_Utils {
std::shared_ptr<Mesh> WhiteCube();

// GLuint eP(GLuint row, GLuint column, GLuint hRes);
// Mesh* WhiteSphere(GLuint hRes = 32, GLuint vRes = 32);
// Mesh* Orbit();
std::shared_ptr<Mesh> Sphere(GLuint hRes = 32, GLuint vRes = 32);

std::shared_ptr<Mesh> TextMesh(const std::string &text, const Font &font);

std::shared_ptr<Mesh> LoadMesh(const std::string& filePath);

std::shared_ptr<Mesh> ColoredBackground(const std::array<glm::vec4, 4> &colors);

// Mesh* ColoredSphere(GLuint hRes = 32, GLuint vRes = 32);
// Mesh* ColoredCube();
}  // namespace Mesh_Utils
