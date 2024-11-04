#include <SDL2/SDL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>

#include "render.hpp"
#include "shader.h"

#ifdef DEBUG
#include "error.h"
#endif

#define VERTEXSIZE 6
#define POSSIZE 3
#define COLORSIZE 3

static int gWidth = 800;
static int gHeight = 600;
static int do_quit = 0;

GLuint vertexNumber;
GLuint elementNumber;

SDL_Window* glWindow = NULL;
SDL_GLContext* glContext = NULL;

// VAO
GLuint glVertexArrayObject = 0;

// VBOs
struct VBOVertex {
    GLuint name;     // VBO name
    GLuint vertIdx;  // Index of the generic vertex attribute for vertices
    GLuint colIdx;   // Index of the generic vertex attribute for color
} glVBOVertex = {0, 0, 1};

// EBO
GLuint glElementBufferObject = 0;

// Grapshics pipeline shader program
GLuint glPipeLineProgram = 0;

struct glUniformMatrix {
    GLint location;
    const char* name;
};

void init() {
    // Initialize program
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("[ERROR] SDL_Init: %s\n", SDL_GetError());
        exit(1);
    }
    // Set attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,
                        SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    glWindow =
        SDL_CreateWindow("OpenGL", 0, 0, gWidth, gHeight, SDL_WINDOW_OPENGL);
    if (glWindow == NULL) {
        printf("[ERROR] SDL_CreateWindow: %s\n", SDL_GetError());
        exit(1);
    }
    glContext = (SDL_GLContext*)SDL_GL_CreateContext(glWindow);
    if (glContext == NULL) {
        printf("[ERROR] SDL_GL_CreateContext: %s\n", SDL_GetError());
        exit(1);
    }

    if (!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        printf("[ERROR] Glad Load Error\n");
        exit(1);
    }
#ifdef DEBUG
    printf("[INFO] Running in debug mode\n");
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(messageCallback, 0);
#endif
}

void vertexSpec() {
    // Vertex specification
    // clang-format off
    const GLfloat vertexData[] = {
         // 0
        -0.5f, -0.5f,  0.5f,  // Vertex position
         1.0f,  0.0f,  0.0f,  // Color
         // 1
         0.5f, -0.5f,  0.5f,
         0.0f,  1.0f,  0.0f,
         // 2
        -0.5f,  0.5f,  0.5f,
         0.0f,  0.0f,  1.0f,
         // 3
         0.5f,  0.5f,  0.5f,
         1.0f,  0.0f,  0.0f,
		 // 4
		 0.5f, -0.5f, -0.5f,
		 0.0f,  1.0f,  0.0f,
		 // 5
		 0.5f,  0.5f, -0.5f,
		 0.0f,  0.0f,  1.0f,
		 // 6
        -0.5f, -0.5f, -0.5f,  
         0.0f,  0.0f,  1.0f,  
         // 7
        -0.5f,  0.5f, -0.5f, 
         0.0f,  1.0f,  0.0f,
    };  // clang-format on
    const GLuint elementData[] = {1, 2, 0, 1, 3, 2, 4, 3, 1, 4, 5, 3,
                                  0, 7, 6, 0, 2, 7, 3, 7, 2, 3, 5, 7,
                                  4, 1, 0, 4, 0, 6, 6, 7, 5, 6, 5, 4};

    vertexNumber = sizeof(vertexData) / sizeof(GLfloat) / VERTEXSIZE;
    elementNumber = sizeof(elementData) / sizeof(GLuint);

    // elementNumber = sizeof(elementData) / sizeof(GLuint);
    printf("[INFO] Number of vertices: %u\n", vertexNumber);

    // Generate VAO
    glGenVertexArrays(1, &glVertexArrayObject);
    // Select VAO
    glBindVertexArray(glVertexArrayObject);
    // Generate VBO
    glGenBuffers(1, &glVBOVertex.name);
    // Select vertex VBO
    glBindBuffer(GL_ARRAY_BUFFER, glVBOVertex.name);

    // Fill vertex VBO with data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData,
                 GL_STATIC_DRAW);

    // EBO
    glGenBuffers(1, &glElementBufferObject);
    // Select EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, glElementBufferObject);
    // Fill EBO with data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementData), elementData,
                 GL_STATIC_DRAW);

    // Set vertex attributes in VBO
    glEnableVertexAttribArray(glVBOVertex.vertIdx);
    glVertexAttribPointer(glVBOVertex.vertIdx, POSSIZE, GL_FLOAT, GL_FALSE,
                          VERTEXSIZE * sizeof(GLfloat), (GLvoid*)0);

    // Set color attributes in the same VBO
    glEnableVertexAttribArray(glVBOVertex.colIdx);
    glVertexAttribPointer(glVBOVertex.colIdx, COLORSIZE, GL_FLOAT, GL_FALSE,
                          VERTEXSIZE * sizeof(GLfloat),
                          (GLvoid*)(POSSIZE * sizeof(GLfloat)));

    // Unbind selected VAO
    // glBindVertexArray(GL_ZERO);
    // glDisableVertexAttribArray(glVBOVertex.idx);
}

void shadersSpec() {
    // Load shaders
    glPipeLineProgram = createShaderProgram(loadShader("shaders/vert.glsl"),
                                            loadShader("shaders/frag.glsl"));
    glUseProgram(glPipeLineProgram);
}

void preDraw() {
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glViewport(0, 0, gWidth, gHeight);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
}

void draw() {
    // Offset variables
    float zOffset = 0.0f;
    float rotationAngleX = 0.0f;
    float rotationAngleY = 0.0f;

    // Retrieve uniforms' locations
    struct glUniformMatrix uTranslation, uPerspective, uRotation;
    uRotation.name = "uRotation";
    uTranslation.name = "uTranslation";
    uPerspective.name = "uPerspective";
    uTranslation.location =
        glGetUniformLocation(glPipeLineProgram, uTranslation.name);
    uPerspective.location =
        glGetUniformLocation(glPipeLineProgram, uPerspective.name);
    uRotation.location =
        glGetUniformLocation(glPipeLineProgram, uRotation.name);

    // Check locations
    if (uTranslation.location < 0) {
        printf("[ERROR] Couldn't find uniform `%s`, location: %d\n",
               uTranslation.name, uTranslation.location);
        quit();
    }
    if (uPerspective.location < 0) {
        printf("[ERROR] Couldn't find uniform `%s`, location: %d\n",
               uPerspective.name, uPerspective.location);
        quit();
    }
    if (uRotation.location < 0) {
        printf("[ERROR] Couldn't find uniform `%s`, location: %d\n",
               uRotation.name, uRotation.location);
        quit();
    }

    // Main loop
    while (!do_quit) {
        // Listen input
        SDL_Event ev;
        while (SDL_PollEvent(&ev) != 0) {
            if (ev.type == SDL_QUIT) {
                do_quit = 1;
            }
        }

        const Uint8* state = SDL_GetKeyboardState(NULL);
        // if (state[SDL_SCANCODE_H]) {
        //     rotationAngleY += 1.0f;
        // }
        // if (state[SDL_SCANCODE_L]) {
        //     rotationAngleY -= 1.0f;
        // }
        // if (state[SDL_SCANCODE_K]) {
        //     rotationAngleX += 1.0f;
        // }
        // if (state[SDL_SCANCODE_J]) {
        //     rotationAngleX -= 1.0f;
        // }

        if (state[SDL_SCANCODE_K]) {
            zOffset += 0.01f;
        }
        if (state[SDL_SCANCODE_J]) {
            zOffset -= 0.01f;
        }

        rotationAngleX += 1.0f;
        rotationAngleY += 1.0f;

        // Build matrices
        glm::mat4 translate = glm::translate(
            glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, zOffset - 3.0f));
        glm::mat4 perspective = glm::perspective(
            glm::radians(45.0f), (float)gWidth / (float)gHeight, 0.1f, 10.0f);
        glm::mat4 rotationY =
            glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngleY),
                        glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rotationX =
            glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngleX),
                        glm::vec3(1.0f, 0.0f, 0.0f));

        glm::mat4 rotation = rotationY * rotationX;

        // Set uniforms
        glUniformMatrix4fv(uRotation.location, 1, GL_FALSE, &rotation[0][0]);
        glUniformMatrix4fv(uTranslation.location, 1, GL_FALSE,
                           &translate[0][0]);
        glUniformMatrix4fv(uPerspective.location, 1, GL_FALSE,
                           &perspective[0][0]);

        // Draw
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        glDrawElements(GL_TRIANGLES, elementNumber, GL_UNSIGNED_INT, 0);

        // Update window
        SDL_GL_SwapWindow(glWindow);
    }
}

void quit() {
    glDeleteBuffers(1, &glVBOVertex.name);
    glDeleteBuffers(1, &glElementBufferObject);
    glDeleteVertexArrays(1, &glVertexArrayObject);

    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(glWindow);
    SDL_Quit();
}

void getInfo() {
    printf("[INFO] Vendor: %s\n", glGetString(GL_VENDOR));
    printf("[INFO] Renderer: %s\n", glGetString(GL_RENDERER));
    printf("[INFO] Version: %s\n", glGetString(GL_VERSION));
    printf("[INFO] Shading language: %s\n",
           glGetString(GL_SHADING_LANGUAGE_VERSION));
}

int main() {
    init();

    getInfo();

    vertexSpec();

    shadersSpec();

    preDraw();

    draw();

    quit();

    return 0;
}
