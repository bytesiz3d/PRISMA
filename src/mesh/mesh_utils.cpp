#pragma once

#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh_utils.hpp"

// ASSIMP
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/Exporter.hpp>

// #define BLACK    000,000,000,255
// #define RED      255,000,000,255
// #define GREEN    000,255,000,255
// #define BLUE     000,000,255,255
// #define YELLOW   255,255,000,255
// #define MAGENTA  255,000,255,255
// #define CYAN     000,255,255,255
// #define WHITE    255,255,255,255

// TODO: Modify each function to contain texture coordinates

Mesh* Mesh_Utils::WhiteCube() {
  Mesh* mesh = new Mesh;
  const int CUBE_SIDES = 6;
  const int CUBE_VERTICES = 4;

  const std::array<glm::vec3, CUBE_SIDES * CUBE_VERTICES> positions = {
    //Upper Face
    glm::vec3(-0.5, 1.0, -0.5),
    glm::vec3(-0.5, 1.0, 0.5),
    glm::vec3(0.5, 1.0, 0.5),
    glm::vec3(0.5, 1.0, -0.5),
    //Lower Face
    glm::vec3(-0.5, 0.0, -0.5),
    glm::vec3(0.5, 0.0, -0.5),
    glm::vec3(0.5, 0.0, 0.5),
    glm::vec3(-0.5, 0.0, 0.5),
    //Right Face
    glm::vec3(0.5, 0.0, -0.5),
    glm::vec3(0.5, 1.0, -0.5),
    glm::vec3(0.5, 1.0, 0.5),
    glm::vec3(0.5, 0.0, 0.5),
    //Left Face
    glm::vec3(-0.5, 0.0, -0.5),
    glm::vec3(-0.5, 0.0, 0.5),
    glm::vec3(-0.5, 1.0, 0.5),
    glm::vec3(-0.5, 1.0, -0.5),
    //Front Face
    glm::vec3(-0.5, 0.0, 0.5),
    glm::vec3(0.5, 0.0, 0.5),
    glm::vec3(0.5, 1.0, 0.5),
    glm::vec3(-0.5, 1.0, 0.5),
    //Back Face
    glm::vec3(-0.5, 0.0, -0.5),
    glm::vec3(-0.5, 1.0, -0.5),
    glm::vec3(0.5, 1.0, -0.5),
    glm::vec3(0.5, 0.0, -0.5)
  };

  const std::array<glm::vec2, CUBE_SIDES * CUBE_VERTICES> texcoords = {
    // Upper Face
    glm::vec2(0.f, 1.f),
    glm::vec2(0.f, 0.f),
    glm::vec2(1.f, 0.f),
    glm::vec2(1.f, 1.f),

    // Lower Face
    glm::vec2(0.f, 0.f),
    glm::vec2(1.f, 0.f),
    glm::vec2(1.f, 1.f),
    glm::vec2(0.f, 1.f),

    // Right Face
    glm::vec2(1.f, 0.f),
    glm::vec2(1.f, 1.f),
    glm::vec2(0.f, 1.f),
    glm::vec2(0.f, 0.f),

    // Left Face
    glm::vec2(0.f, 0.f),
    glm::vec2(1.f, 0.f),
    glm::vec2(1.f, 1.f),
    glm::vec2(0.f, 1.f),

    // Front Face
    glm::vec2(0.f, 0.f),
    glm::vec2(1.f, 0.f),
    glm::vec2(1.f, 1.f),
    glm::vec2(0.f, 1.f),

    // Back Face
    glm::vec2(1.f, 0.f),
    glm::vec2(1.f, 1.f),
    glm::vec2(0.f, 1.f),
    glm::vec2(0.f, 0.f),
  };

  const std::array<glm::vec3, CUBE_SIDES * CUBE_VERTICES> normals = {
    //Upper Face
    glm::vec3(0, 1, 0),
    glm::vec3(0, 1, 0),
    glm::vec3(0, 1, 0),
    glm::vec3(0, 1, 0),
    //Lower Face
    glm::vec3(0, -1, 0),
    glm::vec3(0, -1, 0),
    glm::vec3(0, -1, 0),
    glm::vec3(0, -1, 0),
    //Right Face
    glm::vec3(1, 0, 0),
    glm::vec3(1, 0, 0),
    glm::vec3(1, 0, 0),
    glm::vec3(1, 0, 0),
    //Left Face
    glm::vec3(-1, 0, 0),
    glm::vec3(-1, 0, 0),
    glm::vec3(-1, 0, 0),
    glm::vec3(-1, 0, 0),
    //Front Face
    glm::vec3(0, 0, 1),
    glm::vec3(0, 0, 1),
    glm::vec3(0, 0, 1),
    glm::vec3(0, 0, 1),
    //Back Face
    glm::vec3(0, 0, -1),
    glm::vec3(0, 0, -1),
    glm::vec3(0, 0, -1),
    glm::vec3(0, 0, -1)
  };

  mesh->SetBufferData("positions", sizeof(GLfloat) * positions.size() * positions[0].length(), glm::value_ptr(positions.front()), GL_STATIC_DRAW);
  mesh->SetBufferData("texcoords", sizeof(GLfloat) * texcoords.size() * texcoords[0].length(), glm::value_ptr(texcoords.front()), GL_STATIC_DRAW);
  mesh->SetBufferData("normals", sizeof(GLfloat) * normals.size() * normals[0].length(), glm::value_ptr(normals.front()), GL_STATIC_DRAW);

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
    20, 21, 22, 22, 23, 20};
  mesh->SetElementsData(sizeof(GLuint) * 36, elements, GL_STATIC_DRAW, 36, GL_UNSIGNED_INT);

  // Setting the mesh's AABB:
  for (const auto& position: positions) {
    mesh->AABB_min = glm::min(mesh->AABB_min, position);
    mesh->AABB_max = glm::max(mesh->AABB_max, position);
  }

  return mesh;
}

Mesh* Mesh_Utils::ColoredBackground(const std::array<glm::vec4, 4> &colors) {
  Mesh* mesh = new Mesh;
  constexpr std::array<glm::vec3, 4> positions = {
    glm::vec3(-1.5, -1.5, 1),
    glm::vec3(1.5, -1.5, 1),
    glm::vec3(1.5, 1.5, 1),
    glm::vec3(-1.5, 1.5, 1)
  };

  constexpr std::array<GLuint, 6> indices = { 0, 1, 2,
                                              0, 2, 3 };

  mesh->SetBufferData("positions", sizeof(GLfloat) * positions.size() * positions[0].length(), glm::value_ptr(positions.front()), GL_STATIC_DRAW);
  mesh->SetBufferData("colors", sizeof(GLfloat) * colors.size() * colors[0].length(), glm::value_ptr(colors.front()), GL_STATIC_DRAW);
  mesh->SetElementsData(sizeof(GLuint) * indices.size() , indices.data(), GL_STATIC_DRAW, indices.size(), GL_UNSIGNED_INT);

  mesh->AABB_min = glm::vec3(-1, -1, 0);
  mesh->AABB_max = glm::vec3(1, 1, 0);
  return mesh;
}


Mesh* Mesh_Utils::LoadMesh(const std::string& filePath) {
  // Load scene
  Assimp::Importer importer;

#ifdef DEBUG
  Assimp::DefaultLogger::create(nullptr, Assimp::Logger::VERBOSE, aiDefaultLogStream_STDERR);
  importer.SetPropertyBool(AI_CONFIG_GLOB_MEASURE_TIME, true);
#endif

#ifdef PRISMA_ASSIMP_EXPORT_OBJECTS
  constexpr int importFlags =   aiProcess_Triangulate
                              | aiProcess_JoinIdenticalVertices
                              | aiProcess_ValidateDataStructure
                              | aiProcess_FindInvalidData | aiProcess_GenSmoothNormals
                              | aiProcess_RemoveRedundantMaterials
                              | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph
                              | aiProcess_FixInfacingNormals
                              | aiProcess_GenUVCoords;
#else
  constexpr int importFlags = 0;
#endif

  const aiScene* scene = importer.ReadFile(filePath, importFlags);

  // Check for errors
  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)  // if is Not Zero
  {
    std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
    return nullptr;
  }

#ifdef PRISMA_ASSIMP_EXPORT_OBJECTS
  Assimp::Exporter exporter;

  if (exporter.Export(scene, "assbin", filePath + "_output.assbin") == AI_SUCCESS) {
    std::cout << "Successfully exported " << filePath << " to " << filePath + "_output.assbin" << std::endl;
  } else {
    std::cout << "ERROR::ASSIMP:: " << exporter.GetErrorString() << std::endl;
  }
  return nullptr;  // We don't need the mesh, just exporting it
#endif


  // TODO: directory and texture loading?????????????

  // Allocate space for meshes
  Mesh* mesh = new Mesh;
  std::vector<glm::vec3> positions, normals;
  std::vector<glm::vec2> texcoords;
  std::vector<GLuint> indices;

  for (GLuint m = 0; m < scene->mNumMeshes; m++) {
    aiMesh* meshData = scene->mMeshes[m];
    for (GLuint i = 0; i < meshData->mNumVertices; i++) {
      positions.emplace_back(meshData->mVertices[i].x, meshData->mVertices[i].y, meshData->mVertices[i].z);

      normals.emplace_back(meshData->mNormals[i].x, meshData->mNormals[i].y, meshData->mNormals[i].z);

      if (meshData->mTextureCoords[0]) {
        texcoords.emplace_back(meshData->mTextureCoords[0][i].x, meshData->mTextureCoords[0][i].y);
      }
    }

    for (GLuint i = 0; i < meshData->mNumFaces; i++) {
      aiFace face = meshData->mFaces[i];
      for (GLuint j = 0; j < face.mNumIndices; j++)
        indices.push_back(face.mIndices[j]);
    }
  }

  std::cout << positions.size() << std::endl;
  std::cout << normals.size() << std::endl;
  std::cout << texcoords.size() << std::endl;
  std::cout << indices.size() << std::endl;

  mesh->SetBufferData("positions", sizeof(GLfloat) * positions.size() * positions[0].length(), glm::value_ptr(positions.front()), GL_STATIC_DRAW);
  mesh->SetBufferData("normals", sizeof(GLfloat) * normals.size() * normals[0].length(), glm::value_ptr(normals.front()), GL_STATIC_DRAW);
  mesh->SetBufferData("texcoords", sizeof(GLfloat) * texcoords.size() * texcoords[0].length(), glm::value_ptr(texcoords.front()), GL_STATIC_DRAW);
  mesh->SetElementsData(sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW, indices.size(), GL_UNSIGNED_INT);

  // Setting the mesh's AABB:
  for (const auto& position: positions) {
    mesh->AABB_min = glm::min(mesh->AABB_min, position);
    mesh->AABB_max = glm::max(mesh->AABB_max, position);
  }
  return mesh;
}

Mesh* Mesh_Utils::TextMesh(const std::string& text, const Font& font) {
  Mesh* mesh = new Mesh;

  // Initialize the containers for our data
  std::vector<glm::vec3> positions;
  std::vector<glm::vec2> texcoords;
  std::vector<GLuint> indices;

  GLuint lastIndex = 0;
  float offsetX = 0, offsetY = 0;
  for (char c: text) {
    const auto glyphInfo = font.GetGlyphInfo(c, offsetX, offsetY);
    offsetX = glyphInfo.offsetX;
    offsetY = glyphInfo.offsetY;

    positions.insert(positions.end(), glyphInfo.positions.begin(), glyphInfo.positions.end());
    texcoords.insert(texcoords.end(), glyphInfo.uvs.begin(), glyphInfo.uvs.end());

    indices.push_back(lastIndex);
    indices.push_back(lastIndex + 1);
    indices.push_back(lastIndex + 2);
    indices.push_back(lastIndex);
    indices.push_back(lastIndex + 2);
    indices.push_back(lastIndex + 3);

    lastIndex += 4;
  }

  mesh->SetBufferData("positions", sizeof(GLfloat) * positions.size() * positions[0].length(), glm::value_ptr(positions.front()), GL_STATIC_DRAW);
  mesh->SetBufferData("texcoords", sizeof(GLfloat) * texcoords.size() * texcoords[0].length(), glm::value_ptr(texcoords.front()), GL_STATIC_DRAW);

  mesh->SetElementsData(sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW, indices.size(), GL_UNSIGNED_INT);

  // Setting the mesh's AABB:
  for (const auto& position: positions) {
    mesh->AABB_min = glm::min(mesh->AABB_min, position);
    mesh->AABB_max = glm::max(mesh->AABB_max, position);
  }

  return mesh;
}

Mesh* Mesh_Utils::Sphere(GLuint hRes, GLuint vRes) {
  Mesh* mesh = new Mesh;
  const double sph_PI = std::acos(-1);

  std::vector<glm::vec3> positions, normals;
  std::vector<glm::vec2> texcoords;
  std::vector<GLuint> indices;

  double theta, cos_theta, sin_theta;
  double phi, cos_phi, sin_phi;
  for (GLuint i = 0; i <= vRes; i++) {
    theta = (double)i / vRes * sph_PI;
    cos_theta = std::cos(theta);
    sin_theta = std::sin(theta);

    for (GLuint j = 0; j <= hRes; j++) {
      phi = (double)j / hRes * 2 * sph_PI;
      cos_phi = std::cos(phi);
      sin_phi = std::sin(phi);

      positions.emplace_back(sin_theta * sin_phi, cos_theta, sin_theta * cos_phi);

      texcoords.emplace_back((float)i / vRes, (float)j / hRes);

      normals.emplace_back(sin_theta * sin_phi, cos_theta, sin_theta * cos_phi);
    }
  }

  GLuint index = 0;
  for (GLuint i = 0; i < vRes; i++)
    for (GLuint j = 0; j < hRes; j++) {
      index = j + i * (hRes + 1);

      indices.push_back(index);
      indices.push_back(index + hRes + 1);
      indices.push_back(index + hRes + 2);

      indices.push_back(index + hRes + 2);
      indices.push_back(index + 1);
      indices.push_back(index);
    }

  mesh->SetBufferData("positions", sizeof(GLfloat) * positions.size()* positions[0].length(), glm::value_ptr(positions.front()), GL_STATIC_DRAW);
  mesh->SetBufferData("texcoords", sizeof(GLfloat) * texcoords.size() * texcoords[0].length(), glm::value_ptr(texcoords.front()), GL_STATIC_DRAW);
  mesh->SetBufferData("normals", sizeof(GLfloat) * normals.size() * normals[0].length(), glm::value_ptr(normals.front()), GL_STATIC_DRAW);
  mesh->SetElementsData(sizeof(GLuint) * indices.size(), indices.data(), GL_STATIC_DRAW, indices.size(), GL_UNSIGNED_INT);

  mesh->AABB_min = glm::vec3(-1.f, -1.f, -1.f);
  mesh->AABB_max = glm::vec3(1.f, 1.f, 1.f);

  return mesh;
}
