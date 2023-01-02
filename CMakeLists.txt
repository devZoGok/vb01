cmake_minimum_required(VERSION 3.2)
project(vb01)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -pedantic")
set(BUILD_SHARED_LIBS OFF)
set(BUILD_TESTS OFF)
set(BUILD_SAMPLES OFF)
set(BUILD_WITH_ASSIMP OFF)
set(LIB_NAME vb01)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

cmake_policy(SET CMP0015 NEW)

set(GUI text.cpp)
set(UTILS util.cpp)
set(MATH quaternion.cpp vector.cpp matrix.cpp ray.cpp)
set(RENDER lineRenderer.cpp particleEmitter.cpp camera.cpp light.cpp material.cpp mesh.cpp meshData.cpp model.cpp node.cpp quad.cpp box.cpp root.cpp shader.cpp texture.cpp)
set(ANIM animationController.cpp animationChannel.cpp animation.cpp animatable.cpp keyframeChannel.cpp driver.cpp)
set(ARMATURE skeleton.cpp bone.cpp ikSolver.cpp)
set(ASSET_MANAGER assetManager.cpp textAsset.h imageAsset.h modelAsset.h fontAsset.h)
set(ASSET_READERS imageReader.cpp fontReader.cpp modelReader.cpp xmlModelReader.cpp)

if(BUILD_WITH_ASSIMP)
	set(ASSET_READERS ${ASSET_READERS} assimpModelReader.cpp)
endif()

set(LIB_SRC ${RENDER} ${MATH} ${UTILS} ${GUI} ${ARMATURE} ${ASSET_MANAGER} ${ASSET_READERS} ${ANIM})


include_directories(external/tinydir)
include_directories(external/glm)
include_directories(external/glm/glm)
include_directories(external/glad)
include_directories(external/stb)

add_library(${LIB_NAME} SHARED external/glad/glad.c ${LIB_SRC})

set(TINYXML2_DIR external/tinyxml2)
set(GLFW_DIR external/glfw)
set(FREETYPE_DIR external/freetype)
set(ASSIMP_DIR external/assimp)
set(CPPUNIT_DIR external/CppUnit)

if(UNIX)
	message("Building for UNIX")
elseif(WIN32)
	message("Building for Windows")
endif()

add_subdirectory(${TINYXML2_DIR})
set(TINYXML2_LIB_DIR build/${TINYXML2_DIR})
target_include_directories(${LIB_NAME} PUBLIC ${TINYXML2_DIR})
target_link_directories(${LIB_NAME} PUBLIC ${TINYXML2_LIB_DIR})

add_subdirectory(${GLFW_DIR})
set(GLFW_LIB_DIR build/${GLFW_DIR}/src)
target_include_directories(${LIB_NAME} PUBLIC ${GLFW_DIR}/include/GLFW)
target_link_directories(${LIB_NAME} PUBLIC ${GLFW_LIB_DIR})

set(FREETYPE_LIB_DIR build/${FREETYPE_DIR})
add_subdirectory(${FREETYPE_DIR})
target_include_directories(${LIB_NAME} PUBLIC ${FREETYPE_DIR}/include)
target_link_directories(${LIB_NAME} PUBLIC ${FREETYPE_LIB_DIR})

set(DEPS glfw freetype ${CMAKE_DL_LIBS} tinyxml2)

if(${BUILD_WITH_ASSIMP})
	add_subdirectory(${ASSIMP_DIR})
	set(ASSIMP_LIB_DIR build/${ASSIMP_DIR}/lib)
	target_include_directories(${LIB_NAME} PUBLIC ${ASSIMP_DIR}/include/assimp)
	target_link_directories(${LIB_NAME} PUBLIC ${ASSIMP_LIB_DIR})

	set(DEPS assimp ${DEPS})
endif()

target_link_libraries(${LIB_NAME} ${DEPS})

if(BUILD_TESTS)
	if(UNIX)
		include_directories(/usr/include/cppunit)
		link_directories(/usr/lib)
	elseif(WIN32)
		include_directories(C:/Program\ Files\ \(x86\)/cppunit/include)
		link_directories(C:/Program\ Files\ \(x86\)/cppunit/lib)
	endif()
	
	set(RENDER_TEST nodeTest.cpp cameraTest.cpp shaderTest.cpp particleEmitterTest.cpp)
	set(ARMATURE_TEST boneTest.cpp ikSolverTest.cpp)
	set(ANIMATION_TEST animationChannelTest.cpp)
	set(TEST_SRC main.cpp ${LIB_SRC} ${RENDER_TEST} ${ARMATURE_TEST} ${ANIMATION_TEST})
	
	add_executable(vb01Tests ${TEST_SRC})
	target_link_libraries(vb01Tests ${LIB_NAME} ${DEPS} cppunit)
endif()

if(BUILD_SAMPLES)
	add_executable(textSample textSample.cpp)
	target_link_libraries(textSample ${LIB_NAME})

	add_executable(skeletalAnimationSample skeletalAnimationSample.cpp)
	target_link_libraries(skeletalAnimationSample ${LIB_NAME})

	add_executable(morphAnimationSample morphAnimationSample.cpp)
	target_link_libraries(morphAnimationSample ${LIB_NAME})

	add_executable(driverSample driverSample.cpp)
	target_link_libraries(driverSample ${LIB_NAME})

	add_executable(lightSample lightSample.cpp)
	target_link_libraries(lightSample ${LIB_NAME})

	add_executable(shadowSample shadowSample.cpp)
	target_link_libraries(shadowSample ${LIB_NAME})

	add_executable(particleEmitterSample particleEmitterSample.cpp)
	target_link_libraries(particleEmitterSample ${LIB_NAME})

	add_executable(raySample raySample.cpp)
	target_link_libraries(raySample ${LIB_NAME})

	add_executable(pbrSample pbrSample.cpp)
	target_link_libraries(pbrSample ${LIB_NAME})
endif()
