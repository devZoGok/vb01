cmake_minimum_required(VERSION 2.6)
project(vb01)

include_directories(/usr/include/GLFW)
include_directories(/usr/include/glm)
include_directories(/usr/include/glad)

set(CMAKE_BUILD_TYPE Debug)
link_directories(/usr/lib)

add_executable(vb01 main.cpp)
