#pragma once

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader/tiny_obj_loader.h>

#include "mesh_utils.hpp"

// #define BLACK    000,000,000,255
// #define RED      255,000,000,255
// #define GREEN    000,255,000,255
// #define BLUE     000,000,255,255
// #define YELLOW   255,255,000,255
// #define MAGENTA  255,000,255,255
// #define CYAN     000,255,255,255
// #define WHITE    255,255,255,255

// TODO: Modify each function to contain texture coordinates

GLuint Mesh_Utils::eP(GLuint row, GLuint column, GLuint hRes)
{
    return 1
        + row * hRes
        + column % hRes;
}

Mesh* Mesh_Utils::WhiteSphere(GLuint hRes, GLuint vRes)
{
    Mesh* mesh = new Mesh;
    const double sph_PI = std::acos(-1);
    const GLuint vertexCount =
        1
        + hRes * (vRes - 1)
        + 1;

    // Fill with white
    GLubyte* colors = new GLubyte[4 * vertexCount];
    memset(colors, 0xFF, 4 * vertexCount);

    float* positions = new float[3 * vertexCount];
    int pIdx = -1;

    // Top point:
    positions[++pIdx] = 0;
    positions[++pIdx] = 1;
    positions[++pIdx] = 0;

    const double vAngleUnit = sph_PI / vRes;
    const double hAngleUnit = sph_PI * 2 / hRes;

    for (double theta = vAngleUnit; theta < sph_PI; theta += vAngleUnit)
    {
        for (double phi = 0; phi < 2 * sph_PI; phi += hAngleUnit)
        {
            positions[++pIdx] = std::sin(theta) * std::sin(phi);
            positions[++pIdx] = std::cos(theta);
            positions[++pIdx] = std::sin(theta) * std::cos(phi);
        }
    }

    // Bottom point:
    positions[++pIdx] = 0;
    positions[++pIdx] = -1;
    positions[++pIdx] = 0;

    mesh->SetBufferData("positions", sizeof(GLfloat) * 3 * vertexCount, positions, GL_STATIC_DRAW);
    mesh->SetBufferData("colors", sizeof(GLubyte) * 4 * vertexCount, colors, GL_STATIC_DRAW);

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

    mesh->SetElementsData(sizeof(GLuint) * elementCount, elements, GL_STATIC_DRAW, elementCount, GL_UNSIGNED_INT);

    // Setting the mesh's AABB:
    for (GLuint i = 0; i < vertexCount * 3; i += 3)
    {
        mesh->AABB_min[0] = (positions[i + 0] < mesh->AABB_min[0]) ? positions[i + 0] : mesh->AABB_min[0];
        mesh->AABB_min[1] = (positions[i + 1] < mesh->AABB_min[1]) ? positions[i + 1] : mesh->AABB_min[1];
        mesh->AABB_min[2] = (positions[i + 2] < mesh->AABB_min[2]) ? positions[i + 2] : mesh->AABB_min[2];

        mesh->AABB_max[0] = (positions[i + 0] > mesh->AABB_max[0]) ? positions[i + 0] : mesh->AABB_max[0];
        mesh->AABB_max[1] = (positions[i + 1] > mesh->AABB_max[1]) ? positions[i + 1] : mesh->AABB_max[1];
        mesh->AABB_max[2] = (positions[i + 2] > mesh->AABB_max[2]) ? positions[i + 2] : mesh->AABB_max[2];
    }

    delete[] positions;
    delete[] colors;
    delete[] elements;
        
    return mesh;
}

Mesh* Mesh_Utils::Orbit()
{
    const double __PI = std::acos(-1);
    Mesh* mesh = new Mesh;

    float positions[360 * 3];
    GLubyte colors[360 * 4];
    memset(colors, 0xFF, 4 * 360);

    int pIdx = -1;
    for (GLuint theta = 0; theta < 360; theta++) {
        positions[++pIdx] = std::cos(__PI * theta / 180);
        positions[++pIdx] = 0;
        positions[++pIdx] = std::sin(__PI * theta / 180);
    }

    GLuint elements[360];
    for (GLuint idx = 0; idx < 360; idx++)
        elements[idx] = idx;

    mesh->SetBufferData("positions", sizeof(float) * 360 * 3, positions, GL_STATIC_DRAW);
    mesh->SetBufferData("colors", sizeof(GLubyte) * 360 * 4, colors, GL_STATIC_DRAW);
    mesh->SetElementsData(sizeof(GLuint) * 360, elements, GL_STATIC_DRAW, 360, GL_UNSIGNED_INT);

    // Setting the mesh's AABB:
    for (GLuint i = 0; i < 360 * 3; i += 3)
    {
        mesh->AABB_min[0] = (positions[i + 0] < mesh->AABB_min[0]) ? positions[i + 0] : mesh->AABB_min[0];
        mesh->AABB_min[1] = (positions[i + 1] < mesh->AABB_min[1]) ? positions[i + 1] : mesh->AABB_min[1];
        mesh->AABB_min[2] = (positions[i + 2] < mesh->AABB_min[2]) ? positions[i + 2] : mesh->AABB_min[2];

        mesh->AABB_max[0] = (positions[i + 0] > mesh->AABB_max[0]) ? positions[i + 0] : mesh->AABB_max[0];
        mesh->AABB_max[1] = (positions[i + 1] > mesh->AABB_max[1]) ? positions[i + 1] : mesh->AABB_max[1];
        mesh->AABB_max[2] = (positions[i + 2] > mesh->AABB_max[2]) ? positions[i + 2] : mesh->AABB_max[2];
    }

    return mesh;
}

Mesh* Mesh_Utils::WhiteCube()
{
    Mesh* mesh = new Mesh;
    float positions[3*24] = {
        //Upper Face
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

    GLubyte colors[4*24];
    memset(colors, 0xFF, 4 * 24);

    float texCoords[2*24] = {
        // Upper Face
        0.f, 1.f,
        0.f, 0.f,
        1.f, 0.f,
        1.f, 1.f,

        // Lower Face
        0.f, 0.f,
        1.f, 0.f,
        1.f, 1.f,
        0.f, 1.f,
        
        // Right Face
        1.f, 0.f,
        1.f, 1.f,
        0.f, 1.f,
        0.f, 0.f,

        // Left Face
        0.f, 0.f,
        1.f, 0.f,
        1.f, 1.f,
        0.f, 1.f,

        // Front Face
        0.f, 0.f,
        1.f, 0.f,
        1.f, 1.f,
        0.f, 1.f,

        // Back Face
        1.f, 0.f,
        1.f, 1.f,
        0.f, 1.f,
        0.f, 0.f,
    };

    mesh->SetBufferData("positions", sizeof(GLfloat) * 3 * 24, positions, GL_STATIC_DRAW);
    mesh->SetBufferData("colors", sizeof(GLubyte) * 4 * 24, colors, GL_STATIC_DRAW);
    mesh->SetBufferData("texCoords", sizeof(GLfloat) * 2 * 24, texCoords, GL_STATIC_DRAW);

    GLuint elements[36] = {
        //Upper Face
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
    mesh->SetElementsData(sizeof(GLuint) * 36, elements, GL_STATIC_DRAW, 36, GL_UNSIGNED_INT);

    // Setting the mesh's AABB:
    for (GLuint i = 0; i < 24 * 3; i += 3)
    {
        mesh->AABB_min[0] = (positions[i + 0] < mesh->AABB_min[0]) ? positions[i + 0] : mesh->AABB_min[0];
        mesh->AABB_min[1] = (positions[i + 1] < mesh->AABB_min[1]) ? positions[i + 1] : mesh->AABB_min[1];
        mesh->AABB_min[2] = (positions[i + 2] < mesh->AABB_min[2]) ? positions[i + 2] : mesh->AABB_min[2];

        mesh->AABB_max[0] = (positions[i + 0] > mesh->AABB_max[0]) ? positions[i + 0] : mesh->AABB_max[0];
        mesh->AABB_max[1] = (positions[i + 1] > mesh->AABB_max[1]) ? positions[i + 1] : mesh->AABB_max[1];
        mesh->AABB_max[2] = (positions[i + 2] > mesh->AABB_max[2]) ? positions[i + 2] : mesh->AABB_max[2];
    }

    return mesh;
}

Mesh* Mesh_Utils::OBJMesh(const std::string& filePath)
{
    Mesh *mesh = new Mesh;

    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    // basepath = nullptr
    // triangluate = false
    bool success = tinyobj::LoadObj(&attributes, &shapes, &materials, &err,
                                    filePath.c_str(), nullptr, false);

    if (!err.empty()) {
        std::cerr << "ERR: " << err << std::endl;
    }

    if (!success) {
        printf("Failed to load/parse .obj.\n");
        return Mesh_Utils::WhiteCube();
    }
    
    // attributes.vertices,
    // attributes.normals,
    // attributes.texcoords
    // are std::vector<float> which contain our data
    GLuint vertexCount = attributes.vertices.size() / 3;

    // Manually creating our color buffer:
    GLubyte* colors = new GLubyte[4 * vertexCount]; 
    memset(colors, 0xFF, 4 * vertexCount);

    float* positions = &attributes.vertices[0];
    float* texCoords = &attributes.texcoords[0];
    
    mesh->SetBufferData("positions", sizeof(GLfloat) * 3 * vertexCount, positions, GL_STATIC_DRAW);
    mesh->SetBufferData("colors", sizeof(GLubyte) * 4 * vertexCount, colors, GL_STATIC_DRAW);
    mesh->SetBufferData("texCoords", sizeof(GLfloat) * attributes.texcoords.size(), texCoords, GL_STATIC_DRAW);

    // Collect our element indices
    // TODO: Generalize for mesh types that aren't triangluar
    std::vector<GLuint> indices;
    
    // For each shape
    for (size_t i = 0; i < shapes.size(); i++)
    {
        // For every vertex
        for (size_t v = 0; v < shapes[i].mesh.indices.size(); v++)
        {
            tinyobj::index_t idx = shapes[i].mesh.indices[v];
            indices.push_back(idx.vertex_index);
        }
    }

    GLuint* elements = &indices[0];
    mesh->SetElementsData(sizeof(GLuint) * indices.size(), elements, GL_STATIC_DRAW, indices.size(), GL_UNSIGNED_INT);
    
    delete[] colors;

    // Setting the mesh's AABB:
    for (GLuint i = 0; i < vertexCount * 3; i += 3)
    {
        mesh->AABB_min[0] = (positions[i + 0] < mesh->AABB_min[0]) ? positions[i + 0] : mesh->AABB_min[0];
        mesh->AABB_min[1] = (positions[i + 1] < mesh->AABB_min[1]) ? positions[i + 1] : mesh->AABB_min[1];
        mesh->AABB_min[2] = (positions[i + 2] < mesh->AABB_min[2]) ? positions[i + 2] : mesh->AABB_min[2];

        mesh->AABB_max[0] = (positions[i + 0] > mesh->AABB_max[0]) ? positions[i + 0] : mesh->AABB_max[0];
        mesh->AABB_max[1] = (positions[i + 1] > mesh->AABB_max[1]) ? positions[i + 1] : mesh->AABB_max[1];
        mesh->AABB_max[2] = (positions[i + 2] > mesh->AABB_max[2]) ? positions[i + 2] : mesh->AABB_max[2];
    }
    return mesh;
}

Mesh* Mesh_Utils::TextMesh(const std::string& text, Font* font)
{
    Mesh* mesh = new Mesh;

    // Initialize the containers for our data
    std::vector<float> vertices;
    std::vector<float> uvs;
    std::vector<GLuint> indices;
    std::vector<GLubyte> v_colors(4 * 4 * text.length(), 0xFF);

    GLuint lastIndex = 0;
    float offsetX = 0, offsetY = 0;
    for (char c : text)
    {
        const auto glyphInfo = font->GetGlyphInfo(c, offsetX, offsetY);
        offsetX = glyphInfo.offsetX;
        offsetY = glyphInfo.offsetY;

        for (GLubyte i = 0; i < 4; i++)
        {
            for (GLubyte j = 0; j < 3; j++)
                vertices.emplace_back(glyphInfo.positions[i][j]);
            
            for (GLubyte j = 0; j < 2; j++)
                uvs.emplace_back(glyphInfo.uvs[i][j]);
        }

        indices.push_back(lastIndex);
        indices.push_back(lastIndex + 1);
        indices.push_back(lastIndex + 2);
        indices.push_back(lastIndex);
        indices.push_back(lastIndex + 2);
        indices.push_back(lastIndex + 3);

        lastIndex += 4;
    }

    float* positions = &vertices[0];
    float* texCoords = &uvs[0];
    GLubyte* colors = &v_colors[0];


    mesh->SetBufferData("positions", sizeof(GLfloat) * vertices.size(), positions, GL_STATIC_DRAW);
    mesh->SetBufferData("colors", sizeof(GLubyte) * v_colors.size(), colors, GL_STATIC_DRAW);
    mesh->SetBufferData("texCoords", sizeof(GLfloat) * uvs.size(), texCoords, GL_STATIC_DRAW);

    GLuint* elements = &indices[0];
    mesh->SetElementsData(sizeof(GLuint) * indices.size(), elements, GL_STATIC_DRAW, indices.size(), GL_UNSIGNED_INT);
    
    // Setting the mesh's AABB:
    for (GLuint i = 0; i < vertices.size(); i += 3)
    {
        mesh->AABB_min[0] = (positions[i + 0] < mesh->AABB_min[0]) ? positions[i + 0] : mesh->AABB_min[0];
        mesh->AABB_min[1] = (positions[i + 1] < mesh->AABB_min[1]) ? positions[i + 1] : mesh->AABB_min[1];
        mesh->AABB_min[2] = (positions[i + 2] < mesh->AABB_min[2]) ? positions[i + 2] : mesh->AABB_min[2];

        mesh->AABB_max[0] = (positions[i + 0] > mesh->AABB_max[0]) ? positions[i + 0] : mesh->AABB_max[0];
        mesh->AABB_max[1] = (positions[i + 1] > mesh->AABB_max[1]) ? positions[i + 1] : mesh->AABB_max[1];
        mesh->AABB_max[2] = (positions[i + 2] > mesh->AABB_max[2]) ? positions[i + 2] : mesh->AABB_max[2];
    }

    return mesh;
}

// Mesh* Mesh_Utils::ColoredCube()
// {
//     Mesh* mesh = new Mesh;
//     float positions[3*24] = {
//         -1,  1, -1,
//         -1,  1,  1,
//         1,  1,  1,
//         1,  1, -1,
//         //Lower Face
//         -1, -1, -1,
//         1, -1, -1,
//         1, -1,  1,
//         -1, -1,  1,
//         //Right Face
//         1, -1, -1,
//         1,  1, -1,
//         1,  1,  1,
//         1, -1,  1,
//         //Left Face
//         -1, -1, -1,
//         -1, -1,  1,
//         -1,  1,  1,
//         -1,  1, -1,
//         //Front Face
//         -1, -1,  1,
//         1, -1,  1,
//         1,  1,  1,
//         -1,  1,  1,
//         //Back Face
//         -1, -1, -1,
//         -1,  1, -1,
//         1,  1, -1,
//         1, -1, -1
//     };

//     GLubyte colors[4*24] = {
//         //Upper Face
//         RED, RED, RED, RED,
//         //Lower Face
//         GREEN, GREEN, GREEN, GREEN,
//         //Right Face
//         BLUE, BLUE, BLUE, BLUE,
//         //Left Face
//         YELLOW, YELLOW, YELLOW, YELLOW,
//         //Front Face
//         MAGENTA, MAGENTA, MAGENTA, MAGENTA,
//         //Back Face
//         CYAN, CYAN, CYAN, CYAN};

//     mesh->SetBufferData("positions", sizeof(GLfloat) * 3 * 24, positions, GL_STATIC_DRAW);
//     mesh->SetBufferData("colors", sizeof(GLubyte) * 4 * 24, colors, GL_STATIC_DRAW);

//     GLuint elements[36] = {
//         0, 1, 2, 2, 3, 0,
//         //Lower Face
//         4, 5, 6, 6, 7, 4,
//         //Right Face
//         8, 9, 10, 10, 11, 8,
//         //Left Face
//         12, 13, 14, 14, 15, 12,
//         //Front Face
//         16, 17, 18, 18, 19, 16,
//         //Back Face
//         20, 21, 22, 22, 23, 20
//     };

//     mesh->SetElementsData(sizeof(GLuint) * 36, elements, GL_STATIC_DRAW, 36, GL_UNSIGNED_INT);

//     return mesh;
// }

// Mesh* Mesh_Utils::ColoredSphere(GLuint hRes, GLuint vRes)
// {
//     Mesh* mesh = new Mesh;
//     const double sph_PI = std::acos(-1);
//     const GLuint vertexCount =
//         1
//         + hRes * (vRes - 1)
//         + 1;

//     float* positions = new float[3 * vertexCount];
//     int pIdx = -1;

//     GLubyte* colors = new GLubyte[4 * vertexCount];
//     int cIdx = -1;

//     // Top point:
//     positions[++pIdx] = 0;
//     colors[++cIdx] = (positions[pIdx] + 1) / 2 * 0xFF;

//     positions[++pIdx] = 1;
//     colors[++cIdx] = (positions[pIdx] + 1) / 2 * 0xFF;

//     positions[++pIdx] = 0;
//     colors[++cIdx] = (positions[pIdx] + 1) / 2 * 0xFF;
//     colors[++cIdx] = 0xFF;

//     const double vAngleUnit = sph_PI / vRes;
//     const double hAngleUnit = sph_PI * 2 / hRes;

//     for (double theta = vAngleUnit; theta < sph_PI; theta += vAngleUnit)
//     {
//         for (double phi = 0; phi < 2 * sph_PI; phi += hAngleUnit)
//         {
//             positions[++pIdx] = std::sin(theta) * std::sin(phi);
//             colors[++cIdx] = (positions[pIdx] + 1) / 2 * 0xFF;

//             positions[++pIdx] = std::cos(theta);
//             colors[++cIdx] = (positions[pIdx] + 1) / 2 * 0xFF;

//             positions[++pIdx] = std::sin(theta) * std::cos(phi);
//             colors[++cIdx] = (positions[pIdx] + 1) / 2 * 0xFF;
//             colors[++cIdx] = 0xFF;
//         }
//     }

//     // Bottom point:
//     positions[++pIdx] = 0;
//     colors[++cIdx] = (positions[pIdx] + 1) / 2 * 0xFF;

//     positions[++pIdx] = -1;
//     colors[++cIdx] = (positions[pIdx] + 1) / 2 * 0xFF;

//     positions[++pIdx] = 0;
//     colors[++cIdx] = (positions[pIdx] + 1) / 2 * 0xFF;
//     colors[++cIdx] = 0xFF;

//     mesh->SetBufferData("positions", sizeof(GLfloat) * 3 * vertexCount, positions, GL_STATIC_DRAW);
//     mesh->SetBufferData("colors", sizeof(GLubyte) * 4 * vertexCount, colors, GL_STATIC_DRAW);

//     int elementCount = 3 * (
//         hRes
//         + 2 * (hRes * (vRes - 2))
//         + hRes
//         );

//     GLuint* elements = new GLuint[elementCount];

//     int eIdx = -1;

//     // Placing the upper triangle fan:
//     for (GLuint j = 0; j < hRes; j++)
//     {
//         elements[++eIdx] = 0;
//         elements[++eIdx] = eP(0, j, hRes);
//         elements[++eIdx] = eP(0, j + 1, hRes);
//     }

//     // Placing the middle quads:
//     for (GLuint i = 0; i < vRes - 2; i++)
//     {
//         for (GLuint j = 0; j < hRes; j++)
//         {
//             elements[++eIdx] = eP(i, j, hRes);
//             elements[++eIdx] = eP(i + 1, j, hRes);
//             elements[++eIdx] = eP(i + 1, j + 1, hRes);

//             elements[++eIdx] = eP(i + 1, j + 1, hRes);
//             elements[++eIdx] = eP(i, j + 1, hRes);
//             elements[++eIdx] = eP(i, j, hRes);
//         }
//     }

//     // Placing the bottom triangle fan:
//     for (GLuint j = 0; j < hRes; j++)
//     {
//         elements[++eIdx] = vertexCount - 1;
//         elements[++eIdx] = eP(vRes - 2, j, hRes);
//         elements[++eIdx] = eP(vRes - 2, j + 1, hRes);
//     }

//     mesh->SetElementsData(sizeof(GLuint) * elementCount, elements, GL_STATIC_DRAW, elementCount, GL_UNSIGNED_INT);

//     delete[] positions;
//     delete[] colors;
//     delete[] elements;
        
//     return mesh;
// }
