#include <SDL2/SDL.h>
#include "glad/glad.h"

#define VERTEXPOSSIZE 64 

int gWidth = 800; 
int gHeight = 600; 
SDL_Window* glWindow = NULL; 
SDL_GLContext* glContext = NULL; 

/* VAO */ 
GLuint glVertexArrayObject = 0; 
/* VBO */ 
GLuint glVertexBufferObject = 0; 
/* Program object */ 
GLuint glGraphiscPipeLineShaderProgram = 0; 

/* Some data */ 
const char glVertexShaderSource[] = 
	"#version 410 core\n"
	"in vec4 position;\n"
	"void main()\n"
    "{\n"
	"	gl_Position = vec4(position.x, position.y, position.z, position.w);\n"
	"}\n"
	;
	
const char glFragmentShaderSource[] = 
	"#version 410 core\n"
	"out vec4 color;\n"
	"void main()\n"
	"{\n"
	"	color = vec4(0.8f, 0.65f, 0.97f, 1.0f);\n"
	"}\n"
	;
	
GLuint compileShader(GLuint type, const char* source) {
	GLuint shaderObject; 
	switch(type) {
		case GL_VERTEX_SHADER: 
			shaderObject = glCreateShader(GL_VERTEX_SHADER); 
			break; 
		case GL_FRAGMENT_SHADER: 
			shaderObject = glCreateShader(GL_FRAGMENT_SHADER);
			break; 
	}
	glShaderSource(shaderObject, 1, &source, 0); 
	glCompileShader(shaderObject); 
	return shaderObject; 
}

GLuint createShaderProgram(const char* vertexShaderSource, 
						 const char* fragmentShaderSource) { 	
	GLuint programObject = glCreateProgram(); 
	GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource); 
	GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource); 

	glAttachShader(programObject, vertexShader); 
	glAttachShader(programObject, fragmentShader); 
	glLinkProgram(programObject); 

	glValidateProgram(programObject); 
	return programObject; 
	
}

int main() { 
	/* Initialize program */ 
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Init err\n"); 
		exit(1);
	}
	/* Set attributes */ 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1); 

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); 
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); 
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); 

	glWindow = SDL_CreateWindow("OpenGL", 0, 0, gWidth, gHeight, SDL_WINDOW_OPENGL);
	if (glWindow == NULL) {
		printf("glWindow err\n"); 
		exit(1); 
	}
	glContext = SDL_GL_CreateContext(glWindow); 
	if (glContext == NULL) {
		printf("glContext err\n"); 
		exit(1); 
	}

	if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) { 
		printf("glad err\n"); 
		exit(1); 
	}
	
	/* Vertex specification */ 
	const GLfloat vertexPositions[VERTEXPOSSIZE] = {
		-0.8f, -0.8f, 0.0f, 
		0.8f, -0.8f, 0.0f, 
		0.0f, 0.8f, 0.0f
	};
	
	/* Create VAO */ 
	glGenVertexArrays(1, &glVertexArrayObject); 
	glBindVertexArray(glVertexArrayObject); 

	/* Generate VBO */ 
	glGenBuffers(1, &glVertexBufferObject); 
	glBindBuffer(GL_ARRAY_BUFFER, glVertexBufferObject); 
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(GLfloat), vertexPositions, GL_STATIC_DRAW);

	/* Set attributes */ 
	glEnableVertexAttribArray(0); 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); 

	/* Shader source */ 
	glGraphiscPipeLineShaderProgram = createShaderProgram(glVertexShaderSource, 
														  glFragmentShaderSource);
	
	/* Get info */ 
	printf("Vendor: %s\n", glGetString(GL_VENDOR)); 
	printf("Renderer: %s\n", glGetString(GL_RENDERER)); 
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("Shading language: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION)); 

	/* Main loop */ 
	int quit = 0; 
	while (!quit) {
		/* Listen input */
		SDL_Event ev; 
		while (SDL_PollEvent(&ev) != 0) {
			if (ev.type == SDL_QUIT) {
				quit = 1; 
			} 
		}
		
		/* Prepare draw */ 
		glDisable(GL_DEPTH_TEST); 
		glDisable(GL_CULL_FACE); 
		glViewport(0,0, gWidth, gHeight); 
		glClearColor(1.f, 1.f, 0.f, 1.f); 
		glUseProgram(glGraphiscPipeLineShaderProgram); 

		/* Draw */ 
		glBindVertexArray(glVertexArrayObject); 
		glBindBuffer(GL_ARRAY_BUFFER, glVertexBufferObject); 

		glDrawArrays(GL_TRIANGLES, 0, 3); 
		
		SDL_GL_SwapWindow(glWindow); 
	}
	/* Exit program */ 
	glBindVertexArray(0); 
	glDisableVertexAttribArray(0); 

	SDL_DestroyWindow(glWindow); 
	SDL_Quit(); 

	return 0; 
}
