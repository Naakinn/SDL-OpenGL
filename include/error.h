#ifndef ERR_H
#define ERR_H

#ifndef __glad_h_
#include "glad/glad.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define glCheckCall(x) \
    x;                 \
    glGetErrorStatus(#x, __LINE__);

void glGetErrorStatus(const char* function, unsigned int line);
void GLAPIENTRY messageCallback(GLenum source, GLenum type, GLenum id,
                                GLenum severity, GLsizei length,
                                const GLchar* message, const void* userParam);

#ifdef __cplusplus
}
#endif
#endif
