#pragma once

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

Mesh* Mesh_Utils::WhiteCube()
{
    Mesh* mesh = new Mesh;
    float positions[3*24] = {
        //Upper Face
        -0.5,  1.0, -0.5,
        -0.5,  1.0,  0.5,
         0.5,  1.0,  0.5,
         0.5,  1.0, -0.5,
        //Lower Face
        -0.5,  0.0, -0.5,
         0.5,  0.0, -0.5,
         0.5,  0.0,  0.5,
        -0.5,  0.0,  0.5,
        //Right Face
         0.5,  0.0, -0.5,
         0.5,  1.0, -0.5,
         0.5,  1.0,  0.5,
         0.5,  0.0,  0.5,
        //Left Face
        -0.5,  0.0, -0.5,
        -0.5,  0.0,  0.5,
        -0.5,  1.0,  0.5,
        -0.5,  1.0, -0.5,
        //Front Face
        -0.5,  0.0,  0.5,
         0.5,  0.0,  0.5,
         0.5,  1.0,  0.5,
        -0.5,  1.0,  0.5,
        //Back Face
        -0.5,  0.0, -0.5,
        -0.5,  1.0, -0.5,
         0.5,  1.0, -0.5,
         0.5,  0.0, -0.5
    };

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

    float normals[3*24] = {
        //Upper Face
         0,  1,  0,
         0,  1,  0,
         0,  1,  0,
         0,  1,  0,
        //Lower Face
         0, -1,  0,
         0, -1,  0,
         0, -1,  0,
         0, -1,  0,
        //Right Face
         1,  0,  0,
         1,  0,  0,
         1,  0,  0,
         1,  0,  0,
        //Left Face
        -1,  0,  0,
        -1,  0,  0,
        -1,  0,  0,
        -1,  0,  0,
        //Front Face
         0,  0,  1,
         0,  0,  1,
         0,  0,  1,
         0,  0,  1,
        //Back Face
         0,  0, -1,
         0,  0, -1,
         0,  0, -1,
         0,  0, -1        
    };

    mesh->SetBufferData("positions", sizeof(GLfloat) * 3 * 24, positions, GL_STATIC_DRAW);
    mesh->SetBufferData("texCoords", sizeof(GLfloat) * 2 * 24, texCoords, GL_STATIC_DRAW);
    mesh->SetBufferData("normals", sizeof(GLfloat) * 3 * 24, normals, GL_STATIC_DRAW);

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
    // Load scene
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate);

    // Check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return nullptr;
    }
    
    // TODO: directory and texture loading?????????????

    // Allocate space for meshes
    Mesh* mesh = new Mesh;
    std::vector<float> positions, texCoords, normals;
    std::vector<GLuint> indices;

    for (GLuint m = 0; m < scene->mNumMeshes; m++)
    {
        aiMesh* meshData = scene->mMeshes[m];
        for (GLuint i = 0; i < meshData->mNumVertices; i++)
        {
            positions.push_back(meshData->mVertices[i].x);
            positions.push_back(meshData->mVertices[i].y);
            positions.push_back(meshData->mVertices[i].z);

            normals.push_back(meshData->mNormals[i].x);
            normals.push_back(meshData->mNormals[i].y);
            normals.push_back(meshData->mNormals[i].z);

            if (meshData->mTextureCoords[0])
            {
                texCoords.push_back(meshData->mTextureCoords[0][i].x);
                texCoords.push_back(meshData->mTextureCoords[0][i].y);
            }
        }

        for (GLuint i = 0; i < meshData->mNumFaces; i++)
        {
            aiFace face = meshData->mFaces[i];
            for (GLuint j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
    }

    mesh->SetBufferData("positions", sizeof(GLfloat) * positions.size(), positions.data(), GL_STATIC_DRAW);
    mesh->SetBufferData("normals", sizeof(GLfloat) * normals.size(), normals.data(), GL_STATIC_DRAW);
    if (texCoords.empty())
    {
        int texcoords_len = (positions.size() / 3) * 2;
        texCoords.resize(texcoords_len, 0.f);
    }
    mesh->SetBufferData("texCoords", sizeof(GLfloat) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);
    mesh->SetElementsData(sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW, indices.size(), GL_UNSIGNED_INT);

    // Setting the mesh's AABB:
    for (GLuint i = 0; i < positions.size(); i += 3)
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

Mesh* Mesh_Utils::FBXMesh(const std::string& filePath)
{
    // Load scene
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate);

    // Check for errors
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
        return nullptr;
    }
    
    // TODO: directory and texture loading?????????????

    // Allocate space for meshes
    Mesh* mesh = new Mesh;
    std::vector<float> positions, texCoords, normals;
    std::vector<GLuint> indices;

    for (GLuint m = 0; m < scene->mNumMeshes; m++)
    {
        aiMesh* meshData = scene->mMeshes[m];
        for (GLuint i = 0; i < meshData->mNumVertices; i++)
        {
            positions.push_back(meshData->mVertices[i].x);
            positions.push_back(meshData->mVertices[i].y);
            positions.push_back(meshData->mVertices[i].z);

            normals.push_back(meshData->mNormals[i].x);
            normals.push_back(meshData->mNormals[i].y);
            normals.push_back(meshData->mNormals[i].z);

            if (meshData->mTextureCoords[0])
            {
                texCoords.push_back(meshData->mTextureCoords[0][i].x);
                texCoords.push_back(meshData->mTextureCoords[0][i].y);
            }
        }

        for (GLuint i = 0; i < meshData->mNumFaces; i++)
        {
            aiFace face = meshData->mFaces[i];
            for (GLuint j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
    }

    mesh->SetBufferData("positions", sizeof(GLfloat) * positions.size(), &positions[0], GL_STATIC_DRAW);
    mesh->SetBufferData("normals", sizeof(GLfloat) * normals.size(), &normals[0], GL_STATIC_DRAW);
    if (texCoords.empty())
    {
        int texcoords_len = (positions.size() / 3) * 2;
        texCoords.resize(texcoords_len, 0.f);
    }
    mesh->SetBufferData("texCoords", sizeof(GLfloat) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);
    mesh->SetElementsData(sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW, indices.size(), GL_UNSIGNED_INT);

    // Setting the mesh's AABB:
    for (GLuint i = 0; i < positions.size(); i += 3)
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
    std::vector<float> positions;
    std::vector<float> texCoords;
    std::vector<GLuint> indices;

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
                positions.emplace_back(glyphInfo.positions[i][j]);
            
            for (GLubyte j = 0; j < 2; j++)
                texCoords.emplace_back(glyphInfo.uvs[i][j]);
        }

        indices.push_back(lastIndex);
        indices.push_back(lastIndex + 1);
        indices.push_back(lastIndex + 2);
        indices.push_back(lastIndex);
        indices.push_back(lastIndex + 2);
        indices.push_back(lastIndex + 3);

        lastIndex += 4;
    }

    mesh->SetBufferData("positions", sizeof(GLfloat) * positions.size(), &positions[0], GL_STATIC_DRAW);
    mesh->SetBufferData("texCoords", sizeof(GLfloat) * texCoords.size(), &texCoords[0], GL_STATIC_DRAW);

    // Normals
    std::vector<float> normals(positions.size(), 0.f);
    mesh->SetBufferData("normals", sizeof(GLfloat) * normals.size(), &normals[0], GL_STATIC_DRAW);

    mesh->SetElementsData(sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW, indices.size(), GL_UNSIGNED_INT);
    
    // Setting the mesh's AABB:
    for (GLuint i = 0; i < positions.size(); i += 3)
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

Mesh* Mesh_Utils::Sphere(GLuint hRes, GLuint vRes)
{
    Mesh* mesh = new Mesh;
    const double sph_PI = std::acos(-1);

    std::vector<float> positions, texCoords, normals;
    std::vector<GLuint> indices;

    double theta, cos_theta, sin_theta;
    double phi, cos_phi, sin_phi;
    for (GLuint i = 0; i <= vRes; i++)
    {
        theta = (double)i / vRes * sph_PI;
        cos_theta = std::cos(theta);
        sin_theta = std::sin(theta);

        for (GLuint j = 0; j <= hRes; j++)
        {
            phi = (double)j / hRes * 2 * sph_PI;
            cos_phi = std::cos(phi);
            sin_phi = std::sin(phi);

            positions.push_back(sin_theta * sin_phi);
            positions.push_back(cos_theta);
            positions.push_back(sin_theta * cos_phi);
            
            texCoords.push_back((float)i / vRes);
            texCoords.push_back((float)j / hRes);

            normals.push_back(sin_theta * sin_phi);
            normals.push_back(cos_theta);
            normals.push_back(sin_theta * cos_phi);
        }
    }

    GLuint index = 0;
    for (GLuint i = 0; i < vRes; i++)
        for (GLuint j = 0; j < hRes; j++)
        {
            index = j + i * (hRes + 1);

            indices.push_back(index);
            indices.push_back(index + hRes + 1);
            indices.push_back(index + hRes + 2);
            
            indices.push_back(index + hRes + 2);
            indices.push_back(index + 1);
            indices.push_back(index);
        }
    
    mesh->SetBufferData("positions", sizeof(GLfloat) * positions.size(), positions.data(), GL_STATIC_DRAW);
    mesh->SetBufferData("texCoords", sizeof(GLfloat) * texCoords.size(), texCoords.data(), GL_STATIC_DRAW);
    mesh->SetBufferData("normals", sizeof(GLfloat) * normals.size(), normals.data(), GL_STATIC_DRAW);
    mesh->SetElementsData(sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW, indices.size(), GL_UNSIGNED_INT);

    mesh->AABB_min[0] = -1.f;
    mesh->AABB_min[1] = -1.f;
    mesh->AABB_min[2] = -1.f;
    
    mesh->AABB_max[0] = 1.f;
    mesh->AABB_max[1] = 1.f;
    mesh->AABB_max[2] = 1.f;
        
    return mesh;
}

// GLuint Mesh_Utils::eP(GLuint row, GLuint column, GLuint hRes)
// {
//     return 1
//         + row * hRes
//         + column % hRes;
// }

// Mesh* Mesh_Utils::WhiteSphere(GLuint hRes, GLuint vRes)
// {
//     Mesh* mesh = new Mesh;
//     const double sph_PI = std::acos(-1);
//     const GLuint vertexCount =
//         1
//         + hRes * (vRes - 1)
//         + 1;

//     std::vector<float> positions(3 * vertexCount);
//     int pIdx = -1;

//     // Top point:
//     positions[++pIdx] = 0;
//     positions[++pIdx] = 1;
//     positions[++pIdx] = 0;

//     const double vAngleUnit = sph_PI / vRes;
//     const double hAngleUnit = sph_PI * 2 / hRes;

//     for (double theta = vAngleUnit; theta < sph_PI; theta += vAngleUnit)
//     {
//         for (double phi = 0; phi < 2 * sph_PI; phi += hAngleUnit)
//         {
//             positions[++pIdx] = std::sin(theta) * std::sin(phi);
//             positions[++pIdx] = std::cos(theta);
//             positions[++pIdx] = std::sin(theta) * std::cos(phi);
//         }
//     }

//     // Bottom point:
//     positions[++pIdx] = 0;
//     positions[++pIdx] = -1;
//     positions[++pIdx] = 0;

//     mesh->SetBufferData("positions", sizeof(GLfloat) * positions.size(), positions.data(), GL_STATIC_DRAW);

//     int elementCount = 3 * (
//         hRes
//         + 2 * (hRes * (vRes - 2))
//         + hRes
//         );

//     std::vector<GLuint> elements(elementCount);
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

//     mesh->SetElementsData(sizeof(GLuint) * elementCount, elements.data(), GL_STATIC_DRAW, elementCount, GL_UNSIGNED_INT);

//     // Setting the mesh's AABB:
//     for (GLuint i = 0; i < vertexCount * 3; i += 3)
//     {
//         mesh->AABB_min[0] = (positions[i + 0] < mesh->AABB_min[0]) ? positions[i + 0] : mesh->AABB_min[0];
//         mesh->AABB_min[1] = (positions[i + 1] < mesh->AABB_min[1]) ? positions[i + 1] : mesh->AABB_min[1];
//         mesh->AABB_min[2] = (positions[i + 2] < mesh->AABB_min[2]) ? positions[i + 2] : mesh->AABB_min[2];

//         mesh->AABB_max[0] = (positions[i + 0] > mesh->AABB_max[0]) ? positions[i + 0] : mesh->AABB_max[0];
//         mesh->AABB_max[1] = (positions[i + 1] > mesh->AABB_max[1]) ? positions[i + 1] : mesh->AABB_max[1];
//         mesh->AABB_max[2] = (positions[i + 2] > mesh->AABB_max[2]) ? positions[i + 2] : mesh->AABB_max[2];
//     }

//     return mesh;
// }

