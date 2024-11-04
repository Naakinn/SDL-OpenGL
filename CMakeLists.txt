cmake_minimum_required(VERSION 3.10)

project(3DEngine)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR})
set(EXECUTABLE_NAME engine)
set(LINKER mold)

option(USE_LINKER_MOLD "Use mold linker instead of standard gnu ld" ON)
option(DEBUG "Build in debug output mode" OFF)

find_package(glm CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)

set(SRC 
	${PROJECT_SOURCE_DIR}/src/main.cpp
	${PROJECT_SOURCE_DIR}/src/error.c
	${PROJECT_SOURCE_DIR}/src/glad.c
	${PROJECT_SOURCE_DIR}/src/shader.c
	)

if(USE_LINKER_MOLD) 
	add_link_options(-fuse-ld=mold)
endif()


add_executable(${EXECUTABLE_NAME} ${SRC})

if(DEBUG) 
	target_compile_definitions(${EXECUTABLE_NAME} PRIVATE "DEBUG")
	target_compile_options(${EXECUTABLE_NAME} PRIVATE -g)
endif()

target_compile_options(${EXECUTABLE_NAME} PRIVATE -Wall)

target_include_directories(${EXECUTABLE_NAME} PUBLIC 
	${PROJECT_SOURCE_DIR}/include 
	/usr/local/include/SDL2/
)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE glm::glm)
target_link_libraries(${EXECUTABLE_NAME} PRIVATE SDL2)
