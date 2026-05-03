cmake_minimum_required(VERSION 3.5)
project(Renderer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -pedantic")
set(LIB_NAME Renderer)
option(BUILD_RENDERER_TESTS   "Build Renderer unit tests"   OFF)
option(BUILD_RENDERER_SAMPLES "Build Renderer sample apps"  ON)
option(BUILD_WITH_ASSIMP      "Enable Assimp model importer" OFF)
option(BUILD_GUI_ELEMENTS     "Build GUI widget components"  ON)

set(CORE_SRC
    Source/Core/Private/util.cpp)

set(MATH_SRC
    Source/Math/Private/quaternion.cpp
    Source/Math/Private/vector.cpp
    Source/Math/Private/matrix.cpp
    Source/Math/Private/rayCaster.cpp)

set(RENDER_SRC
    Source/Rendering/Private/lineRenderer.cpp
    Source/Rendering/Private/particleEmitter.cpp
    Source/Rendering/Private/camera.cpp
    Source/Rendering/Private/light.cpp
    Source/Rendering/Private/material.cpp
    Source/Rendering/Private/mesh.cpp
    Source/Rendering/Private/meshData.cpp
    Source/Rendering/Private/model.cpp
    Source/Rendering/Private/node.cpp
    Source/Rendering/Private/quad.cpp
    Source/Rendering/Private/box.cpp
    Source/Rendering/Private/root.cpp
    Source/Rendering/Private/shader.cpp
    Source/Rendering/Private/texture.cpp)

set(ANIM_SRC
    Source/Animation/Private/animationController.cpp
    Source/Animation/Private/animationChannel.cpp
    Source/Animation/Private/animation.cpp
    Source/Animation/Private/animatable.cpp
    Source/Animation/Private/keyframeChannel.cpp
    Source/Animation/Private/driver.cpp)

set(ARMATURE_SRC
    Source/Armature/Private/skeleton.cpp
    Source/Armature/Private/bone.cpp
    Source/Armature/Private/ikSolver.cpp)

set(ASSETS_SRC
    Source/Assets/Private/assetManager.cpp)

set(LOADERS_SRC
    Source/Loaders/Private/shaderReader.cpp
    Source/Loaders/Private/imageReader.cpp
    Source/Loaders/Private/fontReader.cpp
    Source/Loaders/Private/modelReader.cpp
    Source/Loaders/Private/xmlModelReader.cpp)

set(GUI_SRC Source/GUI/Private/text.cpp)

if(BUILD_GUI_ELEMENTS)
    list(APPEND GUI_SRC
        Source/GUI/Private/abstractGuiManager.cpp
        Source/GUI/Private/button.cpp
        Source/GUI/Private/listbox.cpp
        Source/GUI/Private/textbox.cpp
        Source/GUI/Private/checkbox.cpp
        Source/GUI/Private/slider.cpp)
endif()

if(BUILD_WITH_ASSIMP)
    list(APPEND LOADERS_SRC Source/Loaders/Private/assimpModelReader.cpp)
endif()

set(LIB_SRC
    ${CORE_SRC} ${MATH_SRC} ${RENDER_SRC} ${ANIM_SRC}
    ${ARMATURE_SRC} ${ASSETS_SRC} ${LOADERS_SRC} ${GUI_SRC})

add_library(${LIB_NAME} ${LIB_SRC})

target_include_directories(${LIB_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Core/Public
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Math/Public
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Rendering/Public
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Animation/Public
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Armature/Public
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Assets/Public
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/Loaders/Public
    ${CMAKE_CURRENT_SOURCE_DIR}/Source/GUI/Public)

set(THIRDPARTY_DIR ${CMAKE_SOURCE_DIR}/ThirdParty)

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
        Tests/main.cpp
        Tests/nodeTest.cpp
        Tests/cameraTest.cpp
        Tests/shaderTest.cpp
        Tests/particleEmitterTest.cpp
        Tests/boneTest.cpp
        Tests/ikSolverTest.cpp
        Tests/animationChannelTest.cpp
        Tests/assetManagerTest.cpp)

    add_executable(RendererTests ${TEST_SRC})
    target_link_libraries(RendererTests PRIVATE ${LIB_NAME} cppunit)
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
