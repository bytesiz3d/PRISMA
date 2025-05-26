#ifndef OPENGL_UTILS_H
#define OPENGL_UTILS_H
#include <glad/glad.h>

class OpenglUtils {
  static void APIENTRY debugOutput(GLenum, GLenum, unsigned int, GLenum, GLsizei, const char*, const void*);
public:
  static bool isDebugContextEnabled();
  static void initOpenglDebugOutput();
};

#endif //OPENGL_UTILS_H
