cmake_minimum_required(VERSION 3.22.1)

project(libgl LANGUAGES CXX)

find_package(OpenGL REQUIRED)

set(SRC
	src/Application.cpp
	src/Mesh.cpp
	src/MeshCube.cpp
	src/MeshSphere.cpp
	src/pch.hpp
	src/ShaderBase.cpp
	src/ShaderProgram.cpp
	
	include/Application.hpp
	include/contracts.hpp
	include/glm.hpp
	include/Mesh.hpp
	include/ShaderBase.hpp
	include/ShaderProgram.hpp
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SRC})

add_library(${PROJECT_NAME} STATIC ${SRC})

target_precompile_headers(${PROJECT_NAME} PRIVATE src/pch.hpp)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED true)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/ext/glew-2.1.0/include)
target_include_directories(${PROJECT_NAME} PUBLIC "include")

target_link_libraries(${PROJECT_NAME} PRIVATE CXX_FLAGS)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw glm::glm OpenGL::GL OpenGL::GLU)

