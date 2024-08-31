#include <SDL2/SDL.h>
#include "glad/glad.h"

int gHeight = 480; 
int gWidth = 640; 
SDL_Window* glWindow = NULL; 
SDL_GLContext* glContext = NULL; 

int main() { 
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("Init err\n"); 
		exit(1);
	}
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
	
	printf("Vendor: %s\n", glGetString(GL_VENDOR)); 
	printf("Renderer: %s\n", glGetString(GL_RENDERER)); 
	printf("Version: %s\n", glGetString(GL_VERSION));
	printf("Shading language: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION)); 
	
	int quit = 0; 
	while (!quit) {
		SDL_Event ev; 
		
		while (SDL_PollEvent(&ev) != 0) {
			if (ev.type == SDL_QUIT) {
				quit = 1; 
			} 
		}
		
		SDL_GL_SwapWindow(glWindow); 
	}
	SDL_DestroyWindow(glWindow); 
	SDL_Quit(); 

	return 0; 
}
