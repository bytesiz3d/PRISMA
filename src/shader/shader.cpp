#include "shader.hpp"
#include <fstream>
#include <iostream>

#include <glm/gtc/type_ptr.hpp>

GLuint Shader::currentlyUsedProgramId = -1;

std::string Shader::readFile(const char* filePath) {
  std::string shader = "";

  // Load the file:
  std::ifstream fileStream(filePath, std::ios::in);
  if (!fileStream.is_open()) {
    std::cerr << "Could not read file " << filePath << ". File does not exist.\n";
    return "";
  }

  // Fill the string line by line:
  std::string currentLine = "";
  while (!fileStream.eof()) {
    std::getline(fileStream, currentLine);
    shader.append(currentLine + "\n");
  }

  fileStream.close();
  return shader;
}

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
  // Create and load the two shaders:
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

  const std::string vertexShaderStr = readFile(vertexPath);
  const std::string fragmentShaderStr = readFile(fragmentPath);

  const char* vertexShaderSource = vertexShaderStr.c_str();
  const char* fragmentShaderSource = fragmentShaderStr.c_str();

  GLint compileStatus = GL_FALSE;
  GLint linkStatus = GL_FALSE;
  int logLength = 0;

  // Compile the vertex shader:
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  // Verify that it was compiled successfully:
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &compileStatus);
  glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);

  if (compileStatus == GL_FALSE) {
    std::string vertexShaderError;
    vertexShaderError.resize((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(vertexShader, logLength, NULL, &vertexShaderError[0]);
    std::cout << &vertexShaderError[0] << std::endl;
  }

  // Compile the fragment shader:
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);

  // Verify that it was compiled successfully:
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &compileStatus);
  glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);

  if (compileStatus == GL_FALSE) {
    std::string fragmentShaderError;
    fragmentShaderError.resize((logLength > 1) ? logLength : 1);
    glGetShaderInfoLog(fragmentShader, logLength, NULL, &fragmentShaderError[0]);
    std::cout << &fragmentShaderError[0] << std::endl;
  }

  // Creating and linking the shader program:
  GLuint program = glCreateProgram();
  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);

  // Verify that it linked successfully:
  glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

  if (linkStatus == GL_FALSE) {
    std::string programError;
    programError.resize((logLength > 1) ? logLength : 1);
    glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
    std::cout << &programError[0] << std::endl;
  }

  // Finishing up
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  programId = program;
}

void Shader::use() const {
  glUseProgram(programId);
  Shader::currentlyUsedProgramId = programId;
}

GLuint Shader::getUniformLocation(const char* name) const {
  if (Shader::currentlyUsedProgramId != programId) {
    // If the current user program is not the same as this shader's program, use this shader
    use();
  }

  if (uniformLocations.contains(name)) {
    return uniformLocations[name];
  }
  return uniformLocations[name] = glGetUniformLocation(programId, name);
}

void Shader::setUniform(const char* name, const int value) const {
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform1i(getUniformLocation(name), value);
  }
}

void Shader::setUniform(const char* name, const std::vector<int>& values) const {
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform1iv(uniform_location, values.size(), values.data());
  }
}

void Shader::setUniform(const char* name, const int v0, const int v1) const {
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform2i(uniform_location, v0, v1);
  }
}

void Shader::setUniform(const char* name, const glm::ivec2& value) const {
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform2iv(uniform_location, 1, glm::value_ptr(value));
  }
}

void Shader::setUniform(const char* name, const std::vector<glm::ivec2>& values) const {
  if (values.empty()) return;
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform2iv(uniform_location, values.size(), glm::value_ptr(values.front()));
  }
}

void Shader::setUniform(const char* name, const int v0, const int v1, const int v2) const {
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform3i(uniform_location, v0, v1, v2);
  }
}

void Shader::setUniform(const char* name, const glm::ivec3& value) const {
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform3iv(uniform_location, 1, glm::value_ptr(value));
  }
}

void Shader::setUniform(const char* name, const std::vector<glm::ivec3>& values) const {
  if (values.empty()) return;
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform3iv(uniform_location, values.size(), glm::value_ptr(values.front()));
  }
}

void Shader::setUniform(const char* name, const int v0, const int v1, const int v2, const int v3) const {
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform4i(uniform_location, v0, v1, v2, v3);
  }
}

void Shader::setUniform(const char* name, const glm::ivec4& value) const {
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform4iv(uniform_location, 1, glm::value_ptr(value));
  }
}

void Shader::setUniform(const char* name, const std::vector<glm::ivec4>& values) const {
  if (values.empty()) return;
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform4iv(uniform_location, values.size(), glm::value_ptr(values.front()));
  }
}

void Shader::setUniform(const char* name, const unsigned int value) const {
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform1ui(uniform_location, value);
  }
}

void Shader::setUniform(const char* name, const std::vector<unsigned int>& values) const {
  if (values.empty()) return;
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform1uiv(uniform_location, values.size(), values.data());
  }
}

void Shader::setUniform(const char* name, const unsigned int v0, const unsigned int v1) const {
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform2ui(uniform_location, v0, v1);
  }
}

void Shader::setUniform(const char* name, const glm::uvec2& value) const {
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform2uiv(uniform_location, 1, glm::value_ptr(value));
  }
}

void Shader::setUniform(const char* name, const std::vector<glm::uvec2>& values) const {
  if (values.empty()) return;
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform2uiv(uniform_location, values.size(), glm::value_ptr(values.front()));
  }
}

void Shader::setUniform(const char* name, const unsigned int v0, const unsigned int v1, const unsigned int v2) const {
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform3ui(uniform_location, v0, v1, v2);
  }
}

void Shader::setUniform(const char* name, const glm::uvec3& value) const {
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform3uiv(uniform_location, 1, glm::value_ptr(value));
  }
}

void Shader::setUniform(const char* name, const std::vector<glm::uvec3>& values) const {
  if (values.empty()) return;
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform3uiv(uniform_location, values.size(), glm::value_ptr(values.front()));
  }
}

void Shader::setUniform(const char* name, const unsigned int v0, const unsigned int v1, const unsigned int v2, const unsigned int v3) const {
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform4ui(uniform_location, v0, v1, v2, v3);
  }
}

void Shader::setUniform(const char* name, const glm::uvec4& value) const {
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform4uiv(uniform_location, 1, glm::value_ptr(value));
  }
}

void Shader::setUniform(const char* name, const std::vector<glm::uvec4>& values) const {
  if (values.empty()) return;
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform4uiv(uniform_location, values.size(), glm::value_ptr(values.front()));
  }
}

void Shader::setUniform(const char* name, const float value) const {
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform1f(uniform_location, value);
  }
}

void Shader::setUniform(const char* name, const std::vector<float>& values) const {
  if (values.empty()) return;
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform1fv(uniform_location, values.size(), values.data());
  }
}

void Shader::setUniform(const char* name, const float v0, const float v1) const {
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform2f(uniform_location, v0, v1);
  }
}

void Shader::setUniform(const char* name, const glm::vec2& value) const {
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform2fv(uniform_location, 1, glm::value_ptr(value));
  }
}

void Shader::setUniform(const char* name, const std::vector<glm::vec2>& values) const {
  if (values.empty()) return;
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform2fv(uniform_location, values.size(), glm::value_ptr(values.front()));
  }
}

void Shader::setUniform(const char* name, const float v0, const float v1, const float v2) const {
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform3f(uniform_location, v0, v1, v2);
  }
}

void Shader::setUniform(const char* name, const glm::vec3& value) const {
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform3fv(uniform_location, 1, glm::value_ptr(value));
  }
}

void Shader::setUniform(const char* name, const std::vector<glm::vec3>& values) const {
  if (values.empty()) return;
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform3fv(uniform_location, values.size(), glm::value_ptr(values.front()));
  }
}

void Shader::setUniform(const char* name, const float v0, const float v1, const float v2, const float v3) const {
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform4f(uniform_location, v0, v1, v2, v3);
  }
}

void Shader::setUniform(const char* name, const glm::vec4& value) const {
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform4fv(uniform_location, 1, glm::value_ptr(value));
  }
}

void Shader::setUniform(const char* name, const std::vector<glm::vec4>& values) const {
  if (values.empty()) return;
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniform4fv(uniform_location, values.size(), glm::value_ptr(values.front()));
  }
}

void Shader::setUniform(const char* name, const glm::mat2& value, const bool transpose) const {
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniformMatrix2fv(uniform_location, 1, transpose, glm::value_ptr(value));
  }
}

void Shader::setUniform(const char* name, const std::vector<glm::mat2>& values, const bool transpose) const {
  if (values.empty()) return;
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniformMatrix2fv(uniform_location, values.size(), transpose, glm::value_ptr(values.front()));
  }
}

void Shader::setUniform(const char* name, const glm::mat3& value, const bool transpose) const {
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniformMatrix3fv(uniform_location, 1, transpose, glm::value_ptr(value));
  }
}

void Shader::setUniform(const char* name, const std::vector<glm::mat3>& values, const bool transpose) const {
  if (values.empty()) return;
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniformMatrix3fv(uniform_location, values.size(), transpose, glm::value_ptr(values.front()));
  }
}

void Shader::setUniform(const char* name, const glm::mat4& value, const bool transpose) const {
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniformMatrix4fv(uniform_location, 1, transpose, glm::value_ptr(value));
  }
}

void Shader::setUniform(const char* name, const std::vector<glm::mat4>& values, const bool transpose) const {
  if (values.empty()) return;
  if (const GLuint uniform_location = getUniformLocation(name); uniform_location != -1) {
    glUniformMatrix4fv(uniform_location, values.size(), transpose, glm::value_ptr(values.front()));
  }
}

Shader::~Shader() {
  glDeleteProgram(programId);
  if (Shader::currentlyUsedProgramId == programId) {
    Shader::currentlyUsedProgramId = -1;  // Reset the currently used program ID
  }
}
