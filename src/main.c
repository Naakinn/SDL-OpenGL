#include <SDL2/SDL.h>
#include "shader.h"

#define VERTEXSIZE 6
#define POSSIZE 3
#define COLORSIZE 3


int gWidth = 800; 
int gHeight = 600; 
GLuint vertexNumber; 
GLuint elementNumber; 
SDL_Window* glWindow = NULL; 
SDL_GLContext* glContext = NULL; 

/* VAO */ 
GLuint glVertexArrayObject = 0; 

/* VBOs */ 
struct VBOVertex {
	GLuint name; /* VBO name */
	GLuint vertIdx; /* Index of the generic vertex attribute for vertices */
	GLuint colIdx; /* Index of the generic vertex attribute for color */
}   glVBOVertex = { 0, 0, 1 };

/* EBO */
GLuint glElementBufferObject = 0; 

/* Grapshics pipeline shader program */ 
GLuint glPipeLineProgram = 0; 

void init() { 
	/* Initialize program */ 
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Init err\n"); 
		exit(1);
	}
	/* Set attributes */ 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4); 
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6); 
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
	const GLfloat vertexData[] = {
		 /* 1 */
		-0.5f, -0.5f,  0.0f, /* Vertex position */
		 1.0f,  0.0f,  0.0f, /* Color */
		 /* 2 */
		 0.5f, -0.5f,  0.0f, 
		 0.0f,  1.0f,  0.0f, 
		 /* 3 */
		-0.5f,  0.5f,  0.0f,
	     0.0f,  0.0f,  1.0f, 
		 /* 4 */
		 0.5f,  0.5f,  0.0f, 
		 1.0f,  0.0f,  0.0f, 
	};
	const GLuint elementData[] = { 2, 0, 1, 3, 2, 1 }; 
	
	vertexNumber = sizeof(vertexData) / sizeof(GLfloat) / VERTEXSIZE; 
	elementNumber = sizeof(elementData) / sizeof(GLuint); 
	
	/* elementNumber = sizeof(elementData) / sizeof(GLuint);  */
	printf("Number of vertices: %u\n", vertexNumber); 
	
	/* Generate VAO */ 
	glGenVertexArrays(1, &glVertexArrayObject);
	/* Select VAO */
	glBindVertexArray(glVertexArrayObject); 
	
	
	/* Generate VBO */ 
	glGenBuffers(1, &glVBOVertex.name); 
	/* Select vertex VBO */
	glBindBuffer(GL_ARRAY_BUFFER, glVBOVertex.name); 
	
	/* Fill vertex VBO with data */ 	
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	
	/* EBO */ 
	glGenBuffers(1, &glElementBufferObject);
	/* Select EBO */
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glElementBufferObject); 
	/* Fill EBO with data */ 
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementData), elementData, GL_STATIC_DRAW);
		
	
	/* Set vertex attributes in VBO */ 
	glEnableVertexAttribArray(glVBOVertex.vertIdx); 
	glVertexAttribPointer(glVBOVertex.vertIdx, POSSIZE, GL_FLOAT, GL_FALSE, VERTEXSIZE * sizeof(GLfloat), (GLvoid*)0); 

	/* Set color attributes in the same VBO */ 
	glEnableVertexAttribArray(glVBOVertex.colIdx); 
	glVertexAttribPointer(glVBOVertex.colIdx, COLORSIZE, GL_FLOAT, GL_FALSE, VERTEXSIZE * sizeof(GLfloat), (GLvoid*)(POSSIZE * sizeof(GLfloat))); 
	
	/* Unbind selected VAO */
	/* glBindVertexArray(GL_ZERO);  */
	/* glDisableVertexAttribArray(glVBOVertex.idx);  */
	
}

void shadersSpec() { 
	/* Load shaders */ 
	glPipeLineProgram = createShaderProgram(loadShader("shaders/vert.glsl"), 
												  loadShader("shaders/frag.glsl"));
	glUseProgram(glPipeLineProgram); 
}

void getInfo() {
	printf("Vendor: %s\n", glGetString(GL_VENDOR)); 
	printf("Renderer: %s\n", glGetString(GL_RENDERER)); 
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("Shading language: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION)); 
}

void prepDraw() {
	
	/* Prepare draw */ 
	glDisable(GL_DEPTH_TEST); 
	glDisable(GL_CULL_FACE); 
	glViewport(0,0, gWidth, gHeight); 
	glClearColor(0.0f, 0.0f, 0.0f, 1.f); 
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); 

	getInfo();
}

void draw() {
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
		/* glDrawArrays(GL_TRIANGLES, glVBOVertex.vertIdx, vertexNumber);  */
		glDrawElements(GL_TRIANGLES, elementNumber, GL_UNSIGNED_INT, 0); 
		
		SDL_GL_SwapWindow(glWindow); 
	}
}

void quitSDL() {
	/* Exit program */ 
	glBindVertexArray(GL_ZERO); 
	glDisableVertexAttribArray(glVBOVertex.vertIdx); 

	SDL_DestroyWindow(glWindow); 
	SDL_Quit(); 
}

int main(int argc, char** argv) {
	
	init(); 
	
	vertexSpec(); 
	
	shadersSpec(); 
	
	prepDraw(); 
	
	draw(); 
	
	quitSDL(); 
	
	return 0; 
}
