#pragma once
#include <cmath>
#include "mesh.hpp"
namespace Mesh_Utils
{
    Mesh CreateMesh();

    GLuint eP(GLuint row, GLuint column, GLuint hRes);
    
    Mesh ColoredSphere(GLuint hRes = 32, GLuint vRes = 32);
    Mesh ColoredCube();
    Mesh Orbit();
}
