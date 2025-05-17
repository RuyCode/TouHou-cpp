# =============================================================================
# Basic Project Configuration
# =============================================================================
cmake_minimum_required(VERSION 3.16)
project(
    TouHouCPP
    VERSION 1.0.0
    LANGUAGES CXX
)

# =============================================================================
# Global Settings
# =============================================================================
set(EDITOR "${CMAKE_PROJECT_NAME}_editor")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =============================================================================
# Protocol Buffers Configuration
# =============================================================================
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

set(
    ${EDITOR}_PROTO_FILES
    "${CMAKE_SOURCE_DIR}/src/proto/game.proto"
)

protobuf_generate_cpp(
    ${EDITOR}_PROTO_SRCS
    ${EDITOR}_PROTO_HDRS
    ${${EDITOR}_PROTO_FILES}
)

add_library(
    ${EDITOR}_PROTO
    STATIC
    ${${EDITOR}_PROTO_SRCS}
    ${${EDITOR}_PROTO_HDRS}
)

# =============================================================================
# Source Files Configuration
# =============================================================================
set(
    ${EDITOR}_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/levelgenerator.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor/mainwindow.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor/scenewidget.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor/datamanager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor/mobinspector.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor/mobbatchinspector.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor/mobprefabinspector.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor/attackinspector.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor/bulletinspector.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor/hierarchywindow.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor/command.cpp"
)

set(
    ${EDITOR}_HEADERS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/levelgenerator.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor/mainwindow.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor/scenewidget.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor/datamanager.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor/mobinspector.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor/mobbatchinspector.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor/mobprefabinspector.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor/attackinspector.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor/attackinspector.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor/hierarchywindow.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/editor/command.h"
)

# =============================================================================
# Qt6 Configuration
# =============================================================================
find_package(
    Qt6
    REQUIRED
    COMPONENTS
        Core
        Widgets
        Gui
        OpenGLWidgets
        OpenGL
)

qt_standard_project_setup()

# =============================================================================
# Executable Target
# =============================================================================
qt_add_executable(
    ${EDITOR}
    ${${EDITOR}_SOURCES}
    ${${EDITOR}_HEADERS}
)

# =============================================================================
# Target Linking
# =============================================================================
target_link_libraries(
    ${EDITOR}
    PRIVATE
        Qt6::Widgets
        Qt6::Core
        Qt6::Gui
        Qt6::OpenGLWidgets
        Qt6::OpenGL
        ${Protobuf_LIBRARIES}
        ${EDITOR}_PROTO
)

# =============================================================================
# Compiler Options and Quality Checks
# =============================================================================
target_compile_options(
    ${EDITOR}
    PRIVATE
        -Wall
        -Wextra
        -Werror
        -Wpedantic
)

if(ENABLE_DEBUG)
    target_compile_options(
        ${EDITOR}
        PRIVATE
            -g
    )

    target_compile_options(
        ${EDITOR}_PROTO
        PRIVATE
            -g
    )
endif()

# Clang-format check target
add_custom_target(
    clang-format
    COMMAND clang-format
        --dry-run
        -Werror
        ${${EDITOR}_SOURCES}
        ${${EDITOR}_HEADERS}
)

# Clang-tidy configuration
set_target_properties(
    ${EDITOR}
    PROPERTIES
        CXX_CLANG_TIDY "clang-tidy"
)

# =============================================================================
# Configuration Files
# =============================================================================
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy"
    "${CMAKE_CURRENT_BINARY_DIR}"
    COPYONLY
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/.clang-format"
    "${CMAKE_CURRENT_BINARY_DIR}"
    COPYONLY
)
