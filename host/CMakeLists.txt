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

if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
	target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/ext/glew-2.1.0/lib/Release/x64/glew32.lib)
	target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/ext/glew-2.1.0/lib/Release/x64/glew32s.lib)

	add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/ext/glew-2.1.0/bin/Release/x64/glew32.dll ${CMAKE_CURRENT_BINARY_DIR}/Debug/glew32.dll
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/ext/glew-2.1.0/bin/Release/x64/glew32.dll ${CMAKE_CURRENT_BINARY_DIR}/Release/glew32.dll
	)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Darwin")
	target_link_libraries(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/ext/glew-2.1.0/lib/Release/mac-m1/libGLEW.a)

	add_custom_command(
		TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/ext/glew-2.1.0/bin/Release/mac-m1/libGLEW.2.1.0.dylib ${CMAKE_CURRENT_BINARY_DIR}/Debug/libGLEW.2.1.0.dylib
		COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_SOURCE_DIR}/ext/glew-2.1.0/bin/Release/mac-m1/libGLEW.2.1.0.dylib ${CMAKE_CURRENT_BINARY_DIR}/Release/libGLEW.2.1.0.dylib
	)
endif()
