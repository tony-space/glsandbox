cmake_minimum_required(VERSION 3.22.1)

project(Sandbox)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#third parties
add_subdirectory(ext)

include(cxxflags.cmake)

add_subdirectory(host)
