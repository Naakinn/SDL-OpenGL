#include "error.h"

#include <stdio.h>

void glGetErrorStatus(const char* function, unsigned int line) {
    GLenum err = 0;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("[ERROR] Error code: %d, at line %u, in `%s`\n", err, line,
               function);
        return;
    }
}

void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLenum id,
                                GLenum severity, GLsizei length,
                                const GLchar* message, const void* userParam) {
    fprintf(stderr,
            "[GL_CALLBACK]: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""), type,
            severity, message);
}
