cmake_minimum_required(VERSION 3.22.1)

project(host LANGUAGES CXX)

set(SRC
	main.cpp
	pch.hpp
)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SRC})

add_executable(${PROJECT_NAME} ${SRC})

target_precompile_headers(${PROJECT_NAME} PRIVATE pch.hpp)

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED true)

target_link_libraries(${PROJECT_NAME} PRIVATE CXX_FLAGS libgl)
