cmake_minimum_required(VERSION 3.28)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(project "TouhouCPP")   
project(${project} LANGUAGES C CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC include)

set(${project}_SOURCES 
    src/main.cpp
    src/core/GameManager.cpp
    src/core/SceneManager.cpp
    src/scenes/MainScene.cpp
    src/graphics/Shader.cpp
    src/graphics/VertexArrayObject.cpp
    src/graphics/VertexBufferObject.cpp
    src/graphics/ElementBufferObject.cpp
    src/graphics/Texture2D.cpp
)   

set(${project}_HEADERS
    src/core/GameManager.h
    src/core/SceneManager.h
    src/scenes/MainScene.h
    src/scenes/Scene.h
    src/graphics/Shader.h
    src/graphics/VertexArrayObject.h
    src/graphics/VertexBufferObject.h
    src/graphics/ElementBufferObject.h
    src/graphics/Texture2D.h
)

set(${project}_SOURCE_LIST
    ${${project}_SOURCES}
    ${${project}_HEADERS}
)

add_executable(${project} ${${project}_SOURCE_LIST})

target_compile_features(${project} PRIVATE cxx_std_17)
target_include_directories(${project} PRIVATE src)
target_link_libraries(${project} PRIVATE SFML::Graphics )
target_link_libraries(${project} PRIVATE glad)