cmake_minimum_required(VERSION 2.6)
project(vb01)

include_directories(/usr/include/GLFW)
include_directories(/usr/include/glm)
include_directories(/usr/include/glad)

set(CMAKE_BUILD_TYPE Debug)
set(math quaternion.cpp vector.cpp)
set(render root.cpp mesh.cpp node.cpp material.cpp texture.cpp camera.cpp light.cpp quad.cpp shader.cpp)
link_directories(/usr/lib)

add_executable(vb01 main.cpp ${math} ${render})
target_link_libraries(vb01 glfw)
target_link_libraries(vb01 glad)
