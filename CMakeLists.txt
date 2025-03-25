cmake_minimum_required(VERSION 3.16)

project(TouHouCPP VERSION 1.0.0 LANGUAGES CXX)
set(EDITOR ${CMAKE_PROJECT_NAME}_editor)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set CXX properties
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_CLANG_TIDY "clang-tidy")

# Find ProtoBuf and generate files
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
set(${EDITOR}_PROTO_FILES
    ${CMAKE_SOURCE_DIR}/src/proto/game.proto
)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${${EDITOR}_PROTO_FILES})

# Set sources and headers
set(${EDITOR}_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/levelgenerator.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/scenewidget.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/datamanager.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/mobinspector.hpp
)
set(${EDITOR}_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/mainwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/scenewidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/levelgenerator.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/datamanager.h
)

# Find and setup Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui OpenGLWidgets OpenGL)
qt_standard_project_setup()

# Add executable
qt_add_executable(${EDITOR}
    ${${EDITOR}_SOURCES}
    ${${EDITOR}_HEADERS}
    ${PROTO_SRCS} ${PROTO_HDRS}
)

# Link Qt6
target_link_libraries(${EDITOR} PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui Qt6::OpenGLWidgets Qt6::OpenGL)
target_link_libraries(${EDITOR} PRIVATE ${Protobuf_LIBRARIES})

# Add checks
target_compile_options(${EDITOR} PRIVATE -Wall -Wextra -Werror -Wpedantic)
if(ENABLE_DEBUG)
    target_compile_options(${EDITOR} PRIVATE -g)
endif()
add_custom_target(
    clang-format
    COMMAND clang-format
    --dry-run
    -Werror
    ${${EDITOR}_SOURCES}
    ${${EDITOR}_HEADERS}
)
