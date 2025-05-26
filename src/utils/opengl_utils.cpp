#include "opengl_utils.h"

#include <iostream>
#include <glad/glad.h>
#include "GLFW/glfw3.h"


void APIENTRY OpenglUtils::debugOutput(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length, const char *message, const void *userParam) {
  std::cerr << "-----------------------------------------" << std::endl;
  std::cerr << "[DEBUG][OPENGL] message (" << id << "): " << message << std::endl;

  switch (source) {
    case GL_DEBUG_SOURCE_API:
      std::cerr << "Source: API";
      break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
      std::cerr << "Source: Window System";
      break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
      std::cerr << "Source: Shader Compiler";
      break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
      std::cerr << "Source: Third Party";
      break;
    case GL_DEBUG_SOURCE_APPLICATION:
      std::cerr << "Source: Application";
      break;
    case GL_DEBUG_SOURCE_OTHER:
      std::cerr << "Source: Other";
      break;
  }
  std::cerr << std::endl;

  switch (type) {
    case GL_DEBUG_TYPE_ERROR:
      std::cerr << "Type: Error";
      break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
      std::cerr << "Type: Deprecated Behaviour";
      break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
      std::cerr << "Type: Undefined Behaviour";
      break;
    case GL_DEBUG_TYPE_PORTABILITY:
      std::cerr << "Type: Portability";
      break;
    case GL_DEBUG_TYPE_PERFORMANCE:
      std::cerr << "Type: Performance";
      break;
    case GL_DEBUG_TYPE_MARKER:
      std::cerr << "Type: Marker";
      break;
    case GL_DEBUG_TYPE_PUSH_GROUP:
      std::cerr << "Type: Push Group";
      break;
    case GL_DEBUG_TYPE_POP_GROUP:
      std::cerr << "Type: Pop Group";
      break;
    case GL_DEBUG_TYPE_OTHER:
      std::cerr << "Type: Other";
      break;
  }

  std::cerr << std::endl;

  switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
      std::cerr << "Severity: high";
      break;
    case GL_DEBUG_SEVERITY_MEDIUM:
      std::cerr << "Severity: medium";
      break;
    case GL_DEBUG_SEVERITY_LOW:
      std::cerr << "Severity: low";
      break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
      std::cerr << "Severity: notification";
      break;
  }
  std::cerr << std::endl << "-----------------------------------------" << std::endl;
}

bool OpenglUtils::isDebugContextEnabled() {
#ifndef DEBUG
  return false; // Debug context is only enabled in debug builds
#endif

  int flags;
  glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
  return flags & GLFW_OPENGL_DEBUG_CONTEXT;
}

void OpenglUtils::initOpenglDebugOutput() {
  if (!isDebugContextEnabled()) {
    std::cerr << "[WARNING] OpenGL debug context is not enabled. Debug output will not be available." << std::endl;
    return;
  }
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(OpenglUtils::debugOutput, nullptr);
  glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
}
