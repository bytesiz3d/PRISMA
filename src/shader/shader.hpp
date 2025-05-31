#pragma once
#include <glad/glad.h>

#include <unordered_map>
#include <string>
#include <vector>

#include "glm/glm.hpp"

class Shader {
    GLuint programId;
    mutable std::unordered_map<std::string, GLuint> uniformLocations;
    static GLuint currentlyUsedProgramId;

    /*
          Loads a file's ASCII content into a string and returns it.
          Returns an empty string if it fails.
        */
    static std::string readFile(const char *filePath);

    GLuint getUniformLocation(const char *name) const;

public:
    /*
          Loads the vertex and fragment shaders from their two respective paths.
          Links the two shaders and returns the compiled program.
        */
    Shader(const char *vertexPath, const char *fragmentPath);

    void use() const;

    void setUniform(const char *name, int value) const;
    void setUniform(const char *name, const std::vector<int>& values) const;

    void setUniform(const char *name, int v0, int v1) const;
    void setUniform(const char *name, const glm::ivec2 &value) const;
    void setUniform(const char *name, const std::vector<glm::ivec2> &values) const;

    void setUniform(const char *name, int v0, int v1, int v2) const;
    void setUniform(const char *name, const glm::ivec3 &value) const;
    void setUniform(const char *name, const std::vector<glm::ivec3> &values) const;

    void setUniform(const char *name, int v0, int v1, int v2, int v3) const;
    void setUniform(const char *name, const glm::ivec4 &value) const;
    void setUniform(const char *name, const std::vector<glm::ivec4> &values) const;

    void setUniform(const char *name, unsigned int value) const;
    void setUniform(const char *name, const std::vector<unsigned int> &values) const;

    void setUniform(const char *name, unsigned int v0, unsigned int v1) const;
    void setUniform(const char *name, const glm::uvec2 &value) const;
    void setUniform(const char *name, const std::vector<glm::uvec2> &values) const;

    void setUniform(const char *name, unsigned int v0, unsigned int v1, unsigned int v2) const;
    void setUniform(const char *name, const glm::uvec3 &value) const;
    void setUniform(const char *name, const std::vector<glm::uvec3> &values) const;

    void setUniform(const char *name, unsigned int v0, unsigned int v1, unsigned int v2, unsigned int v3) const;
    void setUniform(const char *name, const glm::uvec4 &value) const;
    void setUniform(const char *name, const std::vector<glm::uvec4> &values) const;


    void setUniform(const char *name, float value) const;
    void setUniform(const char *name, const std::vector<float> &values) const;

    void setUniform(const char *name, float v0, float v1) const;
    void setUniform(const char *name, const glm::vec2 &value) const;
    void setUniform(const char *name, const std::vector<glm::vec2> &values) const;

    void setUniform(const char *name, float v0, float v1, float v2) const;
    void setUniform(const char *name, const glm::vec3 &value) const;
    void setUniform(const char *name, const std::vector<glm::vec3> &values) const;

    void setUniform(const char *name, float v0, float v1, float v2, float v3) const;
    void setUniform(const char *name, const glm::vec4 &value) const;
    void setUniform(const char *name, const std::vector<glm::vec4> &values) const;

    void setUniform(const char *name, const glm::mat2 &value, bool transpose = false) const;
    void setUniform(const char *name, const std::vector<glm::mat2> &value, bool transpose = false) const;
    void setUniform(const char *name, const glm::mat3 &value, bool transpose = false) const;
    void setUniform(const char *name, const std::vector<glm::mat3> &value, bool transpose = false) const;
    void setUniform(const char *name, const glm::mat4 &value, bool transpose = false) const;
    void setUniform(const char *name, const std::vector<glm::mat4> &value, bool transpose = false) const;


    ~Shader();
}; // class Shader
