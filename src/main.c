#include <SDL2/SDL.h>
#include "shader.h"

#define VERTEXATTRSTRIDE GL_ZERO 
#define VERTEXATTRSIZE 3
#define COLORATTRSIZE 3


int gWidth = 800; 
int gHeight = 600; 
GLuint vertexNumber; 
SDL_Window* glWindow = NULL; 
SDL_GLContext* glContext = NULL; 

/* VAO */ 
GLuint glVertexArrayObject = 0; 

/* Grapshics pipeline shader program */ 
GLuint glPipeLineShaderProgram = 0; 

/* VBOs */ 
struct VBO {
	GLuint name; /* VBO name */
	GLuint idx; /* Index of the generic vertex attribute array */
}   glVBOVertex = { 0, 0 },
	glVBOColor = { 0, 1 };

void init() { 
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
}

void vertexSpec() {
	/* Vertex specification */ 
	const GLfloat vertexPositions[] = {
		-0.8f, -0.8f,  0.0f, 
		 0.0f, -0.8f,  0.0f, 
		-0.4f,  0.0f,  0.0f,
			
		 0.0f, -0.8f,  0.0f, 
		 0.8f, -0.8f,  0.0f, 
		 0.4f,  0.0f,  0.0f,
			
		-0.4f,  0.0f,  0.0f, 
		 0.4f,  0.0f,  0.0f, 
		 0.0f, -0.8f,  0.0f, 
		 
		-0.4f,  0.0f,  0.0f, 
		 0.4f,  0.0f,  0.0f, 
		 0.0f,  0.8f,  0.0f, 
	};
	
	const GLfloat vertexColors[] = {
		 0.0f, 1.0f, 0.0f, 
		 0.0f, 0.0f, 1.0f, 
		 1.0f, 0.0f, 0.0f, 
		 
		 0.0f, 0.0f, 1.0f, 
		 1.0f, 0.0f, 0.0f, 
		 0.0f, 1.0f, 0.0f, 
		 
		 1.0f, 0.0f, 0.0f, 
		 0.0f, 1.0f, 0.0f, 
		 0.0f, 0.0f, 1.0f, 
		 
		 1.0f, 0.0f, 0.0f, 
		 0.0f, 1.0f, 0.0f, 
		 0.0f, 0.0f, 1.0f, 
	};
	vertexNumber = sizeof(vertexPositions) / sizeof(GLfloat); 
	if (sizeof(vertexColors) / sizeof(GLfloat) != vertexNumber) {
		printf("Color or vertex specifications are missing\n");
		exit(1); 
	}  
	vertexNumber /= VERTEXATTRSIZE; 
	
	/* Generate VAO */ 
	glGenVertexArrays(1, &glVertexArrayObject);
	/* Select VAO */
	glBindVertexArray(glVertexArrayObject); 
	
	/* Generate VBOs*/ 
	glGenBuffers(1, &glVBOVertex.name); 
	glGenBuffers(1, &glVBOColor.name); 
	
	
	/* Select vertex VBO */
	glBindBuffer(GL_ARRAY_BUFFER, glVBOVertex.name); 
	
	/* Fill vertex VBO with data */ 	
	glBufferData(GL_ARRAY_BUFFER, vertexNumber * VERTEXATTRSIZE * sizeof(GLfloat), vertexPositions, GL_STATIC_DRAW);

	/* Set attributes for vertex VBO */ 
	glEnableVertexAttribArray(glVBOVertex.idx); 
	glVertexAttribPointer(glVBOVertex.idx, VERTEXATTRSIZE, GL_FLOAT, GL_FALSE, VERTEXATTRSTRIDE, (GLvoid*)0); 

	
	/* Select color VBO */
	glBindBuffer(GL_ARRAY_BUFFER, glVBOColor.name); 

	/* Fill color VBO with data */
	glBufferData(GL_ARRAY_BUFFER, vertexNumber * VERTEXATTRSIZE * sizeof(GLfloat), vertexColors, GL_STATIC_DRAW);
	
	/* Set attributes for color VBO */ 
	glEnableVertexAttribArray(glVBOColor.idx); 
	glVertexAttribPointer(glVBOColor.idx, COLORATTRSIZE, GL_FLOAT, GL_FALSE, VERTEXATTRSTRIDE, (GLvoid*)0); 
	
	
	/* Unbind selected VAO */
	/* glBindVertexArray(GL_ZERO);  */
	/* glDisableVertexAttribArray(glVBOVertex.idx);  */
	
}

void shadersSpec() { 
	/* Load shaders */ 
	glPipeLineShaderProgram = createShaderProgram(loadShader("shaders/vert.glsl"), 
												  loadShader("shaders/frag.glsl"));
	glUseProgram(glPipeLineShaderProgram); 
}

void preRender() {
	/* Get info */ 
	printf("Vendor: %s\n", glGetString(GL_VENDOR)); 
	printf("Renderer: %s\n", glGetString(GL_RENDERER)); 
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("Shading language: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION)); 
	
	/* Prepare draw */ 
	glDisable(GL_DEPTH_TEST); 
	glDisable(GL_CULL_FACE); 
	glViewport(0,0, gWidth, gHeight); 
	glClearColor(0.0f, 0.0f, 0.0f, 1.f); 
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); 

}

void quitSDL() {
	/* Exit program */ 
	glBindVertexArray(GL_ZERO); 
	glDisableVertexAttribArray(glVBOVertex.idx); 
	glDisableVertexAttribArray(glVBOVertex.idx); 

	SDL_DestroyWindow(glWindow); 
	SDL_Quit(); 
}

int main(int argc, char** argv) {
	
	init(); 
	
	vertexSpec(); 
	
	shadersSpec(); 
	
	preRender(); 
	
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
		glDrawArrays(GL_TRIANGLES, glVBOVertex.idx, vertexNumber); 
		
		SDL_GL_SwapWindow(glWindow); 
	}
	quitSDL(); 
	return 0; 
}
