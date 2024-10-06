#ifndef SHADER_H 
#define SHADER_H 

#define INCLUDE_GLAD 
#include "glad/glad.h"

char* loadShader(const char* filepath); 

GLuint compileShader(GLuint type, const char* source);

GLuint createShaderProgram(const char* vertexShaderSource, 
						   const char* fragmentShaderSource); 

#endif
