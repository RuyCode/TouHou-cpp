cmake_minimum_required(VERSION 3.28)

set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(project "TouhouCPP")   
project(${project} LANGUAGES C CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")


set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG master
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

file(GLOB_RECURSE ${project}_SOURCES "src/*.cpp")   

file(GLOB_RECURSE ${project}_HEADERS "src/*.h")

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