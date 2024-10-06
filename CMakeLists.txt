cmake_minimum_required(VERSION 3.10)

project(3DEngine)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
set(EXECUTABLE_NAME engine)

set(SRC 
	${PROJECT_SOURCE_DIR}/src/main.c 
	${PROJECT_SOURCE_DIR}/src/err.c
	${PROJECT_SOURCE_DIR}/src/glad.c
	${PROJECT_SOURCE_DIR}/src/shader.c
	)

add_link_options(-fuse-ld=mold -lSDL2)

add_executable(${EXECUTABLE_NAME} ${SRC})

target_include_directories(${EXECUTABLE_NAME} PUBLIC 
	${PROJECT_SOURCE_DIR}/include 
	/usr/local/include/SDL2/
)

