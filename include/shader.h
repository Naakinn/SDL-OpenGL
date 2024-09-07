#include "glad/glad.h"

char* loadShader(const char* filepath); 

GLuint compileShader(GLuint type, const char* source);

GLuint createShaderProgram(const char* vertexShaderSource, 
						   const char* fragmentShaderSource); 

