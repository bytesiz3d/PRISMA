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

Mesh* Mesh_Utils::DoorWall()
{
    Mesh* mesh = new Mesh;
    float positions[3*32] = {
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
        -0.25,  0.0,  0.5,
        -0.25,  0.5,  0.5,
        0.25,  0.5,  0.5,
        0.25,  0.0,  0.5,
         0.5,  0.0,  0.5,
         0.5,  1.0,  0.5,
        -0.5,  1.0,  0.5,
        //Back Face
        -0.5,  0.0, -0.5,
        -0.5,  1.0, -0.5,
         0.5,  1.0, -0.5,
        0.5,  0.0, -0.5,
        0.25,  0.0, -0.5,
        0.25,  0.5, -0.5,
        -0.25, 0.5, -0.5,
        -0.25, 0.0, -0.5,
    };

    float texcoords[2*32] = {
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
        0.25f, 0.f,
        0.25f, 0.5f,
        0.75f, 0.5f,
        0.75f, 0.f,
        1.f, 0.f,
        1.f, 1.f,
        0.f, 1.f,

        // Back Face
        1.f, 0.f,
        1.f, 1.f,
        0.f, 1.f,
        0.f, 0.f,
        0.25f, 0.f,
        0.25f, 0.5f,
        0.75f, 0.5f,
        0.75f, 0.f,
    };

    float normals[3*32] = {
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
         0,  0,  1,
         0,  0,  1,
         0,  0,  1,
         0,  0,  1,
        //Back Face
         0,  0, -1,
         0,  0, -1,
         0,  0, -1,
         0,  0, -1,
         0,  0, -1,
         0,  0, -1,
         0,  0, -1,
         0,  0, -1,        
    };

    mesh->SetBufferData("positions", sizeof(GLfloat) * 3 * 32, positions, GL_STATIC_DRAW);
    mesh->SetBufferData("texcoords", sizeof(GLfloat) * 2 * 32, texcoords, GL_STATIC_DRAW);
    mesh->SetBufferData("normals", sizeof(GLfloat) * 3 * 32, normals, GL_STATIC_DRAW);

    GLuint elements[60] = {
        //Upper Face
        0, 1, 2, 2, 3, 0,
        //Lower Face
        4, 5, 6, 6, 7, 4,
        //Right Face
        8, 9, 10, 10, 11, 8,
        //Left Face
        12, 13, 14, 14, 15, 12,
        //Front Face
        16, 17, 18, 18, 23, 16,
        18, 19, 22, 22, 23, 18,
        19, 20, 21, 21, 22, 19,
        //Back Face
        24, 25, 30, 30, 31, 24,
        30, 25, 26, 26, 29, 30,
        28, 29, 26, 26, 27, 28,
    };
    mesh->SetElementsData(sizeof(GLuint) * 60, elements, GL_STATIC_DRAW, 60, GL_UNSIGNED_INT);

    mesh->AABB_min[0] = -0.5;
    mesh->AABB_min[1] = 0;
    mesh->AABB_min[2] = -0.5;

    mesh->AABB_max[0] = 0.5;
    mesh->AABB_max[1] = 1;
    mesh->AABB_max[2] = 0.5;

    return mesh;
    
}

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

    float texcoords[2*24] = {
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
    mesh->SetBufferData("texcoords", sizeof(GLfloat) * 2 * 24, texcoords, GL_STATIC_DRAW);
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
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    // basepath = nullptr
    // triangluate = false
    bool success = tinyobj::LoadObj(&attrib, &shapes, &materials, &err,
                                    filePath.c_str(), nullptr, true);

    if (!err.empty())
    {
        std::cerr << "ERR: " << err << std::endl;
    }

    if (!success)
    {
        printf("Failed to load/parse .obj.\n");
        return nullptr;
    }
    
    // attrib.vertices,
    // attrib.normals,
    // attrib.texcoords
    // are std::vector<float> which contain our data
    std::vector<GLuint> indices;

    // TODO: Generalize for mesh types that aren't triangluar
    // For each shape
    for (size_t s = 0; s < shapes.size(); s++)
    {
        // For each face
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
            // For each vertex
            int fv = shapes[s].mesh.num_face_vertices[f];
            for (size_t v = 0; v < fv; v++)
            {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                indices.push_back(idx.vertex_index);
            }
            index_offset += fv;
        }
    }

    Mesh *mesh = new Mesh;
    mesh->SetBufferData("positions", sizeof(GLfloat) * attrib.vertices.size(), attrib.vertices.data(), GL_STATIC_DRAW);
    mesh->SetBufferData("normals", sizeof(GLfloat) * attrib.normals.size(), attrib.normals.data(), GL_STATIC_DRAW);
    mesh->SetBufferData("texcoords", sizeof(GLfloat) * attrib.texcoords.size(), attrib.texcoords.data(), GL_STATIC_DRAW);
    mesh->SetElementsData(sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW, indices.size(), GL_UNSIGNED_INT);


    // Setting the mesh's AABB:
    auto vertices = attrib.vertices.data();
    for (GLuint i = 0; i < attrib.vertices.size(); i += 3)
    {
        mesh->AABB_min[0] = (vertices[i + 0] < mesh->AABB_min[0]) ? vertices[i + 0] : mesh->AABB_min[0];
        mesh->AABB_min[1] = (vertices[i + 1] < mesh->AABB_min[1]) ? vertices[i + 1] : mesh->AABB_min[1];
        mesh->AABB_min[2] = (vertices[i + 2] < mesh->AABB_min[2]) ? vertices[i + 2] : mesh->AABB_min[2];

        mesh->AABB_max[0] = (vertices[i + 0] > mesh->AABB_max[0]) ? vertices[i + 0] : mesh->AABB_max[0];
        mesh->AABB_max[1] = (vertices[i + 1] > mesh->AABB_max[1]) ? vertices[i + 1] : mesh->AABB_max[1];
        mesh->AABB_max[2] = (vertices[i + 2] > mesh->AABB_max[2]) ? vertices[i + 2] : mesh->AABB_max[2];
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
    std::vector<float> positions, texcoords, normals;
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
                texcoords.push_back(meshData->mTextureCoords[0][i].x);
                texcoords.push_back(meshData->mTextureCoords[0][i].y);
            }
        }

        for (GLuint i = 0; i < meshData->mNumFaces; i++)
        {
            aiFace face = meshData->mFaces[i];
            for (GLuint j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
    }

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

    for (GLuint i = 1; i < positions.size(); i += 3)
        positions[i] -= mesh->AABB_min[1];

    mesh->AABB_max[1] -= mesh->AABB_min[1];
    mesh->AABB_min[1] = 0;

    mesh->SetBufferData("positions", sizeof(GLfloat) * positions.size(), positions.data(), GL_STATIC_DRAW);
    mesh->SetBufferData("normals", sizeof(GLfloat) * normals.size(), normals.data(), GL_STATIC_DRAW);
    mesh->SetBufferData("texcoords", sizeof(GLfloat) * texcoords.size(), texcoords.data(), GL_STATIC_DRAW);
    mesh->SetElementsData(sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW, indices.size(), GL_UNSIGNED_INT);

    return mesh;
}


Mesh* Mesh_Utils::TextMesh(const std::string& text, Font* font)
{
    Mesh* mesh = new Mesh;

    // Initialize the containers for our data
    std::vector<float> positions;
    std::vector<float> texcoords;
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
                texcoords.emplace_back(glyphInfo.uvs[i][j]);
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
    mesh->SetBufferData("texcoords", sizeof(GLfloat) * texcoords.size(), &texcoords[0], GL_STATIC_DRAW);

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

    std::vector<float> positions, texcoords, normals;
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
            
            texcoords.push_back((float)i / vRes);
            texcoords.push_back((float)j / hRes);

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
    mesh->SetBufferData("texcoords", sizeof(GLfloat) * texcoords.size(), texcoords.data(), GL_STATIC_DRAW);
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
