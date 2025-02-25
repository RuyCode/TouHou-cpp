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
    SYSTEM
)
FetchContent_MakeAvailable(SFML)

FetchContent_Declare(assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG master
)
FetchContent_MakeAvailable(assimp)

add_library(glad STATIC src/glad.c)
target_include_directories(glad PUBLIC include)

set(${project}_SOURCES 
    src/main.cpp
    src/core/GameManager.cpp
    src/core/SceneManager.cpp
    src/scenes/MainScene.cpp
    src/glGraphics/Shader.cpp
    src/glGraphics/VertexArrayObject.cpp
    src/glGraphics/VertexBufferObject.cpp
    src/glGraphics/ElementBufferObject.cpp
    src/glGraphics/Texture2D.cpp
    src/glGraphics/Camera.cpp
    src/glGraphics/Model.cpp
    src/glGraphics/Mesh.cpp
)   

set(${project}_HEADERS
    src/core/GameManager.h
    src/core/SceneManager.h
    src/scenes/MainScene.h
    src/scenes/Scene.h
    src/glGraphics/Shader.h
    src/glGraphics/VertexArrayObject.h
    src/glGraphics/VertexBufferObject.h
    src/glGraphics/ElementBufferObject.h
    src/glGraphics/Texture2D.h
    src/glGraphics/Camera.h
    src/glGraphics/Model.h
    src/glGraphics/Mesh.h
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
target_link_libraries(${project} PUBLIC assimp)