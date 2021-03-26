cmake_minimum_required(VERSION 2.9)
project(vb01)

set(CMAKE_BUILD_TYPE Debug)
set(STB_HEADERS external/stb/)
#add_subdirectory(src/)
set(INCLUDE_DIR include/)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

cmake_policy(SET CMP0015 NEW)

set(GUI text.cpp)
set(UTILS util.cpp)
set(MATH quaternion.cpp vector.cpp matrix.cpp ray.cpp)
set(RENDER particleEmitter.cpp camera.cpp light.cpp material.cpp mesh.cpp model.cpp node.cpp quad.cpp box.cpp root.cpp shader.cpp texture.cpp)
set(MODEL modelReader.cpp assimpModelReader.cpp vbModelReader.cpp)
set(ANIM animationController.cpp animationChannel.cpp animation.cpp animatable.h keyframeChannel.cpp driver.cpp)
set(ARMATURE skeleton.cpp bone.cpp ikSolver.cpp)
set(LIB_SRC ${RENDER} ${MATH} ${MODEL} ${UTILS} ${GUI} ${ARMATURE} ${ASSET_MANAGER} ${ANIM})

#include_directories(${INCLUDE_DIR})

#include_directories(/usr/include/GLFW)
#include_directories(/usr/include/glm)
#include_directories(/usr/include/assimp)
#include_directories(/usr/include/freetype2)
#include_directories(/usr/include/cppunit)

#link_directories(/usr/lib/)

include_directories(${STB_HEADERS})

find_library(GLAD glad external/glad)
include_directories(external/glad/include/glad)

find_library(GLFW glfw3 external/glfw/src)
include_directories(external/glfw/include/GLFW)

include_directories(external/glm)
include_directories(external/glm/glm)

find_library(FREETYPE freetype external/freetype/objs/.lib)
include_directories(external/freetype/include)

find_library(ASSIMP assimp external/assimp/build/bin)
include_directories(external/assimp/include/assimp)

set(DEPS ${GLAD} ${GLFW} ${ASSIMP} ${FREETYPE} ${CMAKE_DL_LIBS})

add_library(vb01 SHARED ${LIB_SRC})
target_link_libraries(vb01 ${DEPS})

set(RENDER_TEST nodeTest.cpp cameraTest.cpp shaderTest.cpp particleEmitterTest.cpp)
set(ARMATURE_TEST boneTest.cpp ikSolverTest.cpp)
set(MODEL_TEST vbModelReaderTest.cpp)
set(ANIMATION_TEST animationChannelTest.cpp)
set(TEST_SRC main.cpp ${LIB_SRC} ${RENDER_TEST} ${ARMATURE_TEST} ${MODEL_TEST} ${ANIMATION_TEST})

add_executable(vb01Tests ${TEST_SRC})
target_link_libraries(vb01Tests ${DEPS} vb01 cppunit)
