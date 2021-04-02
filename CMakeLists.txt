cmake_minimum_required(VERSION 3.2)
project(vb01)

set(CMAKE_BUILD_TYPE Debug)
set(STB_HEADERS external/stb/)
set(INCLUDE_DIR include/)
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -pedantic")
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

cmake_policy(SET CMP0015 NEW)

set(GUI text.cpp)
set(UTILS util.cpp)
set(MATH quaternion.cpp vector.cpp matrix.cpp ray.cpp)
set(RENDER particleEmitter.cpp camera.cpp light.cpp material.cpp mesh.cpp model.cpp node.cpp quad.cpp box.cpp root.cpp shader.cpp texture.cpp)
set(MODEL modelReader.cpp assimpModelReader.cpp vbModelReader.cpp)
set(ANIM animationController.cpp animationChannel.cpp animation.cpp animatable.h keyframeChannel.cpp driver.cpp)
set(ARMATURE skeleton.cpp bone.cpp ikSolver.cpp)
set(LIB_SRC glad.c ${RENDER} ${MATH} ${MODEL} ${UTILS} ${GUI} ${ARMATURE} ${ASSET_MANAGER} ${ANIM})

include_directories(${STB_HEADERS})

include_directories(external/glm)
include_directories(external/glm/glm)

add_library(${PROJECT_NAME} SHARED ${LIB_SRC})

set(GLFW_DIR external/glfw)
set(FREETYPE_DIR external/freetype)
set(ASSIMP_DIR external/assimp)
set(CPPUNIT_DIR external/CppUnit)

add_subdirectory(${GLFW_DIR})
if(UNIX)
	message("Building for UNIX")
	set(GLFW_LIB_DIR build/${GLFW_DIR}/src)
elseif(WIN32)
	message("Building for Windows")
	set(GLFW_LIB_DIR build/${GLFW_DIR}/src/Debug)
endif()
target_include_directories(${PROJECT_NAME} PUBLIC ${GLFW_DIR}/include/GLFW)
target_link_directories(${PROJECT_NAME} PUBLIC ${GLFW_LIB_DIR})

add_subdirectory(${ASSIMP_DIR})
if(UNIX)
	set(ASSIMP_LIB_DIR build/${ASSIMP_DIR}/lib)
elseif(WIN32)
	set(ASSIMP_LIB_DIR build/${ASSIMP_DIR}/lib/Debug)
endif()
target_include_directories(${PROJECT_NAME} PUBLIC ${ASSIMP_DIR}/include/assimp)
target_link_directories(${PROJECT_NAME} PUBLIC ${ASSIMP_LIB_DIR})

if(UNIX)
	set(FREETYPE_LIB_DIR build/${FREETYPE_DIR})
elseif(WIN32)
	set(FREETYPE_LIB_DIR build/${FREETYPE_DIR}/Debug)
endif()
add_subdirectory(${FREETYPE_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${FREETYPE_DIR}/include)
target_link_directories(${PROJECT_NAME} PUBLIC ${FREETYPE_LIB_DIR})

set(DEPS glfw3 assimp freetype ${CMAKE_DL_LIBS})

target_link_libraries(${PROJECT_NAME} ${DEPS})

set(RENDER_TEST nodeTest.cpp cameraTest.cpp shaderTest.cpp particleEmitterTest.cpp)
set(ARMATURE_TEST boneTest.cpp ikSolverTest.cpp)
set(MODEL_TEST vbModelReaderTest.cpp)
set(ANIMATION_TEST animationChannelTest.cpp)
set(TEST_SRC main.cpp ${LIB_SRC} ${RENDER_TEST} ${ARMATURE_TEST} ${MODEL_TEST} ${ANIMATION_TEST})

add_executable(vb01Tests ${TEST_SRC})

if(UNIX)
	set(CPPUNIT_LIB_DIR build/${CPPUNIT_DIR})
elseif(WIN32)
	set(CPPUNIT_LIB_DIR build/${CPPUNIT_DIR}/Debug)
endif()
add_subdirectory(${CPPUNIT_DIR})
target_include_directories(${PROJECT_NAME} PUBLIC ${CPPUNIT_DIR}/include)
target_link_directories(${PROJECT_NAME} PUBLIC ${CPPUNIT_LIB_DIR})

target_link_libraries(vb01Tests ${PROJECT_NAME} ${DEPS} cppunit)
