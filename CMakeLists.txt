cmake_minimum_required(VERSION 3.10)

project(3DEngine)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
set(EXECUTABLE_NAME engine)
set(LINKER mold)

option(USE_LINKER_MOLD "Use mold linker instead of standard gnu ld" ON)
option(DEBUG "Build in debug output mode" ON)

set(SRC 
	${PROJECT_SOURCE_DIR}/src/main.cpp
	${PROJECT_SOURCE_DIR}/src/err.c
	${PROJECT_SOURCE_DIR}/src/glad.c
	${PROJECT_SOURCE_DIR}/src/shader.c
	)

if(USE_LINKER_MOLD) 
	add_link_options(-fuse-ld=mold -lSDL2)
else()
	add_link_options(-lSDL2)
endif()


add_executable(${EXECUTABLE_NAME} ${SRC})

if(DEBUG) 
	target_compile_definitions(${EXECUTABLE_NAME} PRIVATE "DEBUG")
endif()

target_include_directories(${EXECUTABLE_NAME} PUBLIC 
	${PROJECT_SOURCE_DIR}/include 
	/usr/local/include/SDL2/
)

