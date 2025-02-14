cmake_minimum_required(VERSION 3.28)

set(project "TouhouCPP")   
project(${project} LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

set(${project}_SOURCES 
    src/GameManager.cpp
    src/mainLoop.cpp
    src/SceneManager.cpp
    src/MainScene.cpp
)   

set(${project}_HEADERS
    src/GameManager.h
    src/Scene.h
    src/SceneManager.h
    src/MainScene.h
)

set(${project}_SOURCE_LIST
    ${${project}_SOURCES}
    ${${project}_HEADERS}
)

add_executable(${project} ${${project}_SOURCE_LIST})

target_compile_features(${project} PRIVATE cxx_std_17)
target_link_libraries(${project} PRIVATE SFML::Graphics)