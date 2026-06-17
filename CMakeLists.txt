cmake_minimum_required(VERSION 3.5)
project(vb01)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -pedantic")
set(LIB_NAME vb01)
option(BUILD_RENDERER_TESTS   "Build vb01 unit tests"   OFF)
option(BUILD_RENDERER_SAMPLES "Build vb01 sample apps"  ON)
option(BUILD_WITH_ASSIMP      "Enable Assimp model importer" OFF)
option(BUILD_GUI_ELEMENTS     "Build GUI widget components"  ON)

set(CORE_SRC
    source/utils/util.cpp)

set(MATH_SRC
    source/math/quaternion.cpp
    source/math/vector.cpp
    source/math/matrix.cpp
    source/math/rayCaster.cpp)

set(RENDER_SRC
    source/rendering/lineRenderer.cpp
    source/rendering/particleEmitter.cpp
    source/rendering/camera.cpp
    source/rendering/light.cpp
    source/rendering/material.cpp
    source/rendering/mesh.cpp
    source/rendering/meshData.cpp
    source/rendering/model.cpp
    source/rendering/node.cpp
    source/rendering/quad.cpp
    source/rendering/box.cpp
    source/rendering/root.cpp
    source/rendering/shader.cpp
    source/rendering/texture.cpp)

set(ANIM_SRC
    source/animation/animationController.cpp
    source/animation/animationChannel.cpp
    source/animation/animation.cpp
    source/animation/animatable.cpp
    source/animation/keyframeChannel.cpp
    source/animation/driver.cpp)

set(ARMATURE_SRC
    source/armature/skeleton.cpp
    source/armature/bone.cpp
    source/armature/ikSolver.cpp)

set(ASSETS_SRC
    source/assets/assetManager.cpp)

set(READERS_SRC
    source/readers/shaderReader.cpp
    source/readers/imageReader.cpp
    source/readers/fontReader.cpp
    source/readers/modelReader.cpp
    source/readers/xmlModelReader.cpp)

set(GUI_SRC source/gui/text.cpp)

if(BUILD_GUI_ELEMENTS)
    list(APPEND GUI_SRC
        source/gui/abstractGuiManager.cpp
        source/gui/button.cpp
        source/gui/listbox.cpp
        source/gui/textbox.cpp
        source/gui/checkbox.cpp
        source/gui/slider.cpp)
endif()

if(BUILD_WITH_ASSIMP)
    list(APPEND LOADERS_SRC source/loaders/assimpModelReader.cpp)
endif()

set(LIB_SRC
    ${CORE_SRC} ${MATH_SRC} ${RENDER_SRC} ${ANIM_SRC}
    ${ARMATURE_SRC} ${ASSETS_SRC} ${READERS_SRC} ${GUI_SRC})

add_library(${LIB_NAME} ${LIB_SRC})

target_include_directories(${LIB_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/source/core
    ${CMAKE_CURRENT_SOURCE_DIR}/source/math
    ${CMAKE_CURRENT_SOURCE_DIR}/source/rendering
    ${CMAKE_CURRENT_SOURCE_DIR}/source/animation
    ${CMAKE_CURRENT_SOURCE_DIR}/source/armature
    ${CMAKE_CURRENT_SOURCE_DIR}/source/assets
    ${CMAKE_CURRENT_SOURCE_DIR}/source/readers
    ${CMAKE_CURRENT_SOURCE_DIR}/source/gui)

set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/external)

target_link_libraries(${LIB_NAME} PUBLIC glad)

target_link_libraries(${LIB_NAME} PUBLIC glm stb tinydir)

target_include_directories(${LIB_NAME} PUBLIC ${THIRDPARTY_DIR}/GLFW/include)
target_link_libraries(${LIB_NAME} PUBLIC glfw)

target_include_directories(${LIB_NAME} PUBLIC ${THIRDPARTY_DIR}/tinyxml2)
target_link_libraries(${LIB_NAME} PUBLIC tinyxml2)

target_include_directories(${LIB_NAME} PUBLIC ${THIRDPARTY_DIR}/freetype/include)
target_link_libraries(${LIB_NAME} PUBLIC freetype ${CMAKE_DL_LIBS})

if(BUILD_WITH_ASSIMP)
    target_include_directories(${LIB_NAME} PUBLIC
        ${THIRDPARTY_DIR}/assimp/include)
    target_link_libraries(${LIB_NAME} PUBLIC assimp)
endif()

if(BUILD_RENDERER_TESTS)
    if(UNIX)
        include_directories(/usr/include/cppunit)
        link_directories(/usr/lib)
    elseif(WIN32)
        include_directories("C:/Program Files (x86)/cppunit/include")
        link_directories("C:/Program Files (x86)/cppunit/lib")
    endif()

    set(TEST_SRC
        tests/main.cpp
        tests/nodeTest.cpp
        tests/cameraTest.cpp
        tests/shaderTest.cpp
        tests/particleEmitterTest.cpp
        tests/boneTest.cpp
        tests/ikSolverTest.cpp
        tests/animationChannelTest.cpp
        tests/assetManagerTest.cpp)

    add_executable(vb01Tests ${TEST_SRC})
    target_link_libraries(vb01Tests PRIVATE ${LIB_NAME} cppunit)
endif()

if(BUILD_RENDERER_SAMPLES)
    foreach(SAMPLE
            textSample skeletalAnimationSample morphAnimationSample
            driverSample lightSample shadowSample particleEmitterSample
            rayCasterSample planeRayCastSample pbrSample)
        add_executable(${SAMPLE} Samples/${SAMPLE}.cpp)
        target_link_libraries(${SAMPLE} PRIVATE ${LIB_NAME})
    endforeach()
endif()
