cmake_minimum_required(VERSION 2.6)
project(vb01)

include_directories(/usr/include/GLFW)
include_directories(/usr/include/glad)
include_directories(/usr/include/glm)
include_directories(/usr/include/assimp)

link_directories(/usr/lib/)

set(CMAKE_BUILD_TYPE Debug)
set(math quaternion.cpp vector.cpp)
set(render camera.cpp light.cpp material.cpp mesh.cpp model.cpp node.cpp quad.cpp root.cpp shader.cpp texture.cpp)

add_executable(vb01 main.cpp ${render} ${math})
target_link_libraries(vb01 glfw)
target_link_libraries(vb01 glad)
target_link_libraries(vb01 assimp)
