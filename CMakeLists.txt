cmake_minimum_required(VERSION 3.16)


set(project "touhou_editor")
project(${project} VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui OpenGLWidgets OpenGL)
qt_standard_project_setup()

qt_add_executable(helloworld
    main.cpp
    mainwindow.cpp
    mainwindow.h
    scenewidget.cpp
    scenewidget.h
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui Qt6::OpenGLWidgets Qt6::OpenGL)

# target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -Wpedantic)

if(ENABLE_DEBUG)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -g)
endif()

# add_custom_target(
#     clang-format
#     COMMAND clang-format
#     --dry-run
#     -Werror
#     ${SOURCES}
#     ${HEADERS}
# )

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/.clang-format ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
