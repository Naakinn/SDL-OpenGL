#ifndef ERR_H
#define ERR_H 

#ifndef INCLUDE_GLAD
#include "glad/glad.h"
#endif

#define glCheckCall(x) x; glGetErrorStatus(#x, __LINE__); 

void glGetErrorStatus(const char* function, unsigned int line); 
void GLAPIENTRY messageCallback (
		GLenum source, 
		GLenum type, 
		GLenum id, 
		GLenum severity, 
		GLsizei length, 
		const GLchar* message, 
		const void* userParam );

#endif
