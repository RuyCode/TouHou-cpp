cmake_minimum_required(VERSION 3.16)

project(touhou_editor VERSION 1.0.0 LANGUAGES CXX)

# Set CXX properties
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_CLANG_TIDY "clang-tidy")

# Set sources and headers
set(${CMAKE_PROJECT_NAME}_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/qt/mainwindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/qt/scenewidget.cpp)
set(${CMAKE_PROJECT_NAME}_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/qt/mainwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/qt/scenewidget.h)

# Find and setup Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui OpenGLWidgets OpenGL)
qt_standard_project_setup()

# Add executable
qt_add_executable(${CMAKE_PROJECT_NAME}
    ${${CMAKE_PROJECT_NAME}_SOURCES}
    ${${CMAKE_PROJECT_NAME}_HEADERS}
)

# Link Qt6
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui Qt6::OpenGLWidgets Qt6::OpenGL)

# Add checks
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -Wpedantic)
if(ENABLE_DEBUG)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -g)
endif()
add_custom_target(
    clang-format
    COMMAND clang-format
    --dry-run
    -Werror
    ${SOURCES}
    ${HEADERS}
)

# Copy clang config files
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/.clang-format ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
