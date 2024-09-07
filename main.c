#include <SDL2/SDL.h>
#include "glad/glad.h"

#define VERTEXPOSSIZE 64 
#define VERTEXC 9 
#define VERTEXATTRSIZE 3
#define COLORATTRSIZE 3


int gWidth = 800; 
int gHeight = 600; 
SDL_Window* glWindow = NULL; 
SDL_GLContext* glContext = NULL; 

/* VAO */ 
GLuint glVertexArrayObject = 0; 

/* Grapshics pipeline shader program */ 
GLuint glPipeLineShaderProgram = 0; 

/* VBOs */ 
struct VBO {
	GLuint name; /* VBO name */
	GLuint idx; /* Index in VertexAttribArray */
}   glVBOVertex = { 0, 0 },
	glVBOColor = { 0, 1 };

char* loadShader(const char* filepath) { 
	FILE* istream = fopen(filepath, "r"); 
	
	if (istream == NULL) { 
		printf("error occured while opening shader file\n");
		exit(1); 
	}
	
	size_t size; 
	char c; 
	int i; 
	
	for (size = 0; (c = fgetc(istream)) != EOF; ++size) 
		; 
	char* shaderSource = (char*)malloc(size * sizeof(char) + 1);
	
	if (shaderSource == NULL) {
		printf("malloc err\n"); 
		exit(1); 
	}
	
	rewind(istream); 
	for (i = 0; (c = fgetc(istream)) != EOF; ++i) {
		shaderSource[i] = c; 
	} 
	
	shaderSource[i] = '\0'; 
	
	fclose(istream); 
	return shaderSource;
}
	
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
	free((void*)source); 
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

int main(int argc, char** argv) {
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
		 0.0f,  0.8f, 0.0f
	};
	const GLfloat vertexColors[VERTEXPOSSIZE] = {
		 1.0f, 0.0f, 0.0f, 
		 0.0f, 1.0f, 0.0f, 
		 0.0f, 0.0f, 1.0f
	};
	
	/* Generate VAO */ 
	glGenVertexArrays(1, &glVertexArrayObject);
	/* Select VAO */
	glBindVertexArray(glVertexArrayObject); 

	/* Generate vertex VBO */ 
	glGenBuffers(1, &glVBOVertex.name); 
	/* Select vertex VBO */
	glBindBuffer(GL_ARRAY_BUFFER, glVBOVertex.name); 
	/* Fill vertex VBO with data */ 	
	glBufferData(GL_ARRAY_BUFFER, VERTEXC * sizeof(GLfloat), vertexPositions, GL_STATIC_DRAW);

	/* Set attributes for vertex VBO */ 
	glEnableVertexAttribArray(glVBOVertex.idx); 
	glVertexAttribPointer(glVBOVertex.idx, VERTEXATTRSIZE, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0); 
	
	/* Generate color VBO */
	/* glGenBuffers(1, &glVertexBufferObjectColor);  */
	/* glBindBuffer(GL_ARRAY_BUFFER, glVertexBufferObjectColor);  */
	/* glBufferData(GL_ARRAY_BUFFER, VERTEXC * sizeof(GLfloat), vertexColors, GL_STATIC_DRAW); */
	
	/* Set attributes */ 
	/* glEnableVertexAttribArray(1);  */
	/* glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);  */
	
	/* Unbind selected vertex VBO */
	/* glBindVertexArray(GL_ZERO);  */
	/* glDisableVertexAttribArray(glVBOVertex.idx);  */
	
	/* Load shaders */ 
	glPipeLineShaderProgram = createShaderProgram(loadShader("shaders/vert.glsl"), 
												  loadShader("shaders/frag.glsl"));
	glUseProgram(glPipeLineShaderProgram); 
	
	/* Prepare draw */ 
	glDisable(GL_DEPTH_TEST); 
	glDisable(GL_CULL_FACE); 
	glViewport(0,0, gWidth, gHeight); 
	glClearColor(0.0f, 0.0f, 0.0f, 1.f); 
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); 
	
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
		/* Draw */ 
		glDrawArrays(GL_TRIANGLES, 0, 3); 
		
		SDL_GL_SwapWindow(glWindow); 
	}
	
	/* Exit program */ 
	glBindVertexArray(GL_ZERO); 
	glDisableVertexAttribArray(glVBOVertex.idx); 
	/* glDisableVertexAttribArray(1);  */

	SDL_DestroyWindow(glWindow); 
	SDL_Quit(); 

	return 0; 
}
