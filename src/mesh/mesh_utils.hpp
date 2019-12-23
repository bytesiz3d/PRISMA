#pragma once
#include <cmath>

// ASSIMP
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "mesh.hpp"
#include "../font/font.hpp"

namespace Mesh_Utils
{
    Mesh* WhiteCube();

    // GLuint eP(GLuint row, GLuint column, GLuint hRes);
    // Mesh* WhiteSphere(GLuint hRes = 32, GLuint vRes = 32);
    // Mesh* Orbit();
    Mesh* Sphere(GLuint hRes = 32, GLuint vRes = 32);

    Mesh* TextMesh(const std::string& text, Font* font);

    Mesh* OBJMesh(const std::string& filePath);
    Mesh* FBXMesh(const std::string& filePath);

    // Mesh* ColoredSphere(GLuint hRes = 32, GLuint vRes = 32);
    // Mesh* ColoredCube();
}
