#pragma once
#include "mesh_utils.hpp"

#define BLACK    000,000,000,255
#define RED      255,000,000,255
#define GREEN    000,255,000,255
#define BLUE     000,000,255,255
#define YELLOW   255,255,000,255
#define MAGENTA  255,000,255,255
#define CYAN     000,255,255,255
#define WHITE    255,255,255,255


Mesh Mesh_Utils::CreateMesh()
{
    VertexDescriptor vd[2];
    vd[0] = {
        0,
        "positions",
        3,
        GL_FLOAT,
        false,
        0,
        0
    };

    vd[1] = {
        1,
        "colors",
        4,
        GL_UNSIGNED_BYTE,
        true,
        0,
        0
    };

    return Mesh(vd, 2);
}

GLuint Mesh_Utils::eP(GLuint row, GLuint column, GLuint hRes)
{
    return 1
        + row * hRes
        + column % hRes;
}

Mesh Mesh_Utils::ColoredSphere(GLuint hRes, GLuint vRes)
{
    static Mesh mesh = CreateMesh();
    const double __PI = std::acos(-1);
    const GLuint vertexCount =
        1
        + hRes * (vRes - 1)
        + 1;

    float* positions = new float[3 * vertexCount];
    int pIdx = -1;

    GLubyte* colors = new GLubyte[4 * vertexCount];
    int cIdx = -1;

    // Top point:
    positions[++pIdx] = 0;
    colors[++cIdx] = (positions[pIdx] + 1) / 2 * 0xFF;

    positions[++pIdx] = 1;
    colors[++cIdx] = (positions[pIdx] + 1) / 2 * 0xFF;

    positions[++pIdx] = 0;
    colors[++cIdx] = (positions[pIdx] + 1) / 2 * 0xFF;
    colors[++cIdx] = 0xFF;

    const double vAngleUnit = __PI / vRes;
    const double hAngleUnit = __PI * 2 / hRes;

    for (double theta = vAngleUnit; theta < __PI; theta += vAngleUnit)
    {
        for (double phi = 0; phi < 2 * __PI; phi += hAngleUnit)
        {
            positions[++pIdx] = std::sin(theta) * std::sin(phi);
            colors[++cIdx] = (positions[pIdx] + 1) / 2 * 0xFF;

            positions[++pIdx] = std::cos(theta);
            colors[++cIdx] = (positions[pIdx] + 1) / 2 * 0xFF;

            positions[++pIdx] = std::sin(theta) * std::cos(phi);
            colors[++cIdx] = (positions[pIdx] + 1) / 2 * 0xFF;
            colors[++cIdx] = 0xFF;
        }
    }

    // Bottom point:
    positions[++pIdx] = 0;
    colors[++cIdx] = (positions[pIdx] + 1) / 2 * 0xFF;

    positions[++pIdx] = -1;
    colors[++cIdx] = (positions[pIdx] + 1) / 2 * 0xFF;

    positions[++pIdx] = 0;
    colors[++cIdx] = (positions[pIdx] + 1) / 2 * 0xFF;
    colors[++cIdx] = 0xFF;

    mesh.setBufferData("positions", sizeof(GLfloat) * 3 * vertexCount, positions, GL_STATIC_DRAW);
    mesh.setBufferData("colors", sizeof(GLubyte) * 4 * vertexCount, colors, GL_STATIC_DRAW);

    int elementCount = 3 * (
        hRes
        + 2 * (hRes * (vRes - 2))
        + hRes
        );

    GLuint* elements = new GLuint[elementCount];

    int eIdx = -1;

    // Placing the upper triangle fan:
    for (GLuint j = 0; j < hRes; j++)
    {
        elements[++eIdx] = 0;
        elements[++eIdx] = eP(0, j, hRes);
        elements[++eIdx] = eP(0, j + 1, hRes);
    }

    // Placing the middle quads:
    for (GLuint i = 0; i < vRes - 2; i++)
    {
        for (GLuint j = 0; j < hRes; j++)
        {
            elements[++eIdx] = eP(i, j, hRes);
            elements[++eIdx] = eP(i + 1, j, hRes);
            elements[++eIdx] = eP(i + 1, j + 1, hRes);

            elements[++eIdx] = eP(i + 1, j + 1, hRes);
            elements[++eIdx] = eP(i, j + 1, hRes);
            elements[++eIdx] = eP(i, j, hRes);
        }
    }

    // Placing the bottom triangle fan:
    for (GLuint j = 0; j < hRes; j++)
    {
        elements[++eIdx] = vertexCount - 1;
        elements[++eIdx] = eP(vRes - 2, j, hRes);
        elements[++eIdx] = eP(vRes - 2, j + 1, hRes);
    }

    mesh.setElementsData(sizeof(GLuint) * elementCount, elements, GL_STATIC_DRAW, elementCount, GL_UNSIGNED_INT);

    delete[] positions;
    delete[] colors;
    delete[] elements;
        
    return mesh;
}

Mesh Mesh_Utils::Orbit()
{
    const double __PI = std::acos(-1);
    static Mesh mesh = CreateMesh();

    float positions[360 * 3];
    GLuint elements[360];
    GLubyte colors[360 * 4];

    int pIdx = -1;
    for (GLuint theta = 0; theta < 360; theta++) {
        positions[++pIdx] = std::cos(__PI * theta / 180);
        positions[++pIdx] = 0;
        positions[++pIdx] = std::sin(__PI * theta / 180);
    }

    for (GLuint idx = 0; idx < 360; idx++)
        elements[idx] = idx;

    mesh.setBufferData("positions", sizeof(float) * 360 * 3, positions, GL_STATIC_DRAW);
    mesh.setBufferData("colors", sizeof(GLubyte) * 360 * 4, colors, GL_STATIC_DRAW);
    mesh.setElementsData(sizeof(GLuint) * 360, elements, GL_STATIC_DRAW, 360, GL_UNSIGNED_INT);

    return mesh;
}

Mesh Mesh_Utils::ColoredCube()
{
        static Mesh mesh = CreateMesh();
        float positions[3*24] = {
        -1,  1, -1,
        -1,  1,  1,
         1,  1,  1,
         1,  1, -1,
        //Lower Face
        -1, -1, -1,
         1, -1, -1,
         1, -1,  1,
        -1, -1,  1,
        //Right Face
         1, -1, -1,
         1,  1, -1,
         1,  1,  1,
         1, -1,  1,
        //Left Face
        -1, -1, -1,
        -1, -1,  1,
        -1,  1,  1,
        -1,  1, -1,
        //Front Face
        -1, -1,  1,
         1, -1,  1,
         1,  1,  1,
        -1,  1,  1,
        //Back Face
        -1, -1, -1,
        -1,  1, -1,
         1,  1, -1,
         1, -1, -1
        };

        GLubyte colors[4*24] = {
         //Upper Face
        RED, RED, RED, RED,
        //Lower Face
        GREEN, GREEN, GREEN, GREEN,
        //Right Face
        BLUE, BLUE, BLUE, BLUE,
        //Left Face
        YELLOW, YELLOW, YELLOW, YELLOW,
        //Front Face
        MAGENTA, MAGENTA, MAGENTA, MAGENTA,
        //Back Face
        CYAN, CYAN, CYAN, CYAN};

    mesh.setBufferData("positions", sizeof(GLfloat) * 3 * 24, positions, GL_STATIC_DRAW);
    mesh.setBufferData("colors", sizeof(GLubyte) * 4 * 24, colors, GL_STATIC_DRAW);

    GLuint elements[36] = {
        0, 1, 2, 2, 3, 0,
        //Lower Face
        4, 5, 6, 6, 7, 4,
        //Right Face
        8, 9, 10, 10, 11, 8,
        //Left Face
        12, 13, 14, 14, 15, 12,
        //Front Face
        16, 17, 18, 18, 19, 16,
        //Back Face
        20, 21, 22, 22, 23, 20
    };

    mesh.setElementsData(sizeof(GLuint) * 36, elements, GL_STATIC_DRAW, 36, GL_UNSIGNED_INT);

    return mesh;
}

Mesh Mesh_Utils::WhiteCube()
{
        static Mesh mesh = CreateMesh();
        float positions[3*24] = {
            -0.5,  0.5, -0.5,
            -0.5,  0.5,  0.5,
            0.5,  0.5,  0.5,
            0.5,  0.5, -0.5,
            //Lower Face
            -0.5, -0.5, -0.5,
            0.5, -0.5, -0.5,
            0.5, -0.5,  0.5,
            -0.5, -0.5,  0.5,
            //Right Face
            0.5, -0.5, -0.5,
            0.5,  0.5, -0.5,
            0.5,  0.5,  0.5,
            0.5, -0.5,  0.5,
            //Left Face
            -0.5, -0.5, -0.5,
            -0.5, -0.5,  0.5,
            -0.5,  0.5,  0.5,
            -0.5,  0.5, -0.5,
            //Front Face
            -0.5, -0.5,  0.5,
            0.5, -0.5,  0.5,
            0.5,  0.5,  0.5,
            -0.5,  0.5,  0.5,
            //Back Face
            -0.5, -0.5, -0.5,
            -0.5,  0.5, -0.5,
            0.5,  0.5, -0.5,
            0.5, -0.5, -0.5
        };

        GLubyte colors[4*24] = {
            //Upper Face
            WHITE, WHITE, WHITE, WHITE,
            //Lower Face
            WHITE, WHITE, WHITE, WHITE,
            //Right Face
            WHITE, WHITE, WHITE, WHITE,
            //Left Face
            WHITE, WHITE, WHITE, WHITE,
            //Front Face
            WHITE, WHITE, WHITE, WHITE,
            //Back Face
            WHITE, WHITE, WHITE, WHITE
        };

    mesh.setBufferData("positions", sizeof(GLfloat) * 3 * 24, positions, GL_STATIC_DRAW);
    mesh.setBufferData("colors", sizeof(GLubyte) * 4 * 24, colors, GL_STATIC_DRAW);

    GLuint elements[36] = {
        0, 1, 2, 2, 3, 0,
        //Lower Face
        4, 5, 6, 6, 7, 4,
        //Right Face
        8, 9, 10, 10, 11, 8,
        //Left Face
        12, 13, 14, 14, 15, 12,
        //Front Face
        16, 17, 18, 18, 19, 16,
        //Back Face
        20, 21, 22, 22, 23, 20
    };

    mesh.setElementsData(sizeof(GLuint) * 36, elements, GL_STATIC_DRAW, 36, GL_UNSIGNED_INT);

    return mesh;
}
