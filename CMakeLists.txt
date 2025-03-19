cmake_minimum_required(VERSION 3.16)

project(TouhouCPP VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set CXX properties
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# set(CMAKE_CXX_CLANG_TIDY "clang-tidy")

# Find ProtoBuf and generate files
# find_package(Protobuf REQUIRED)
# include_directories(${Protobuf_INCLUDE_DIRS})
# set(PROTO_FILES
#     ${CMAKE_SOURCE_DIR}/src/proto/game.proto
# )
# protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

# Set sources and headers
set(${CMAKE_PROJECT_NAME}_EDITOR_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/qt/mainwindow.cpp
)
set(${CMAKE_PROJECT_NAME}_EDITOR_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/qt/mainwindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/qt/scenewidget.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/editor/game.pb.h
)

# Find and setup Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui OpenGLWidgets OpenGL)
qt_standard_project_setup()

# Add executable
qt_add_executable(${CMAKE_PROJECT_NAME}
    ${${CMAKE_PROJECT_NAME}_EDITOR_SOURCES}
    ${${CMAKE_PROJECT_NAME}_EDITOR_HEADERS}
    # ${PROTO_SRCS} ${PROTO_HDRS}
)

# Link Qt6 and ProtoBuf
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt6::Widgets Qt6::Core Qt6::Gui Qt6::OpenGLWidgets Qt6::OpenGL)
# target_link_libraries(${CMAKE_PROJECT_NAME} ${Protobuf_LIBRARIES})

# Add checks
target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -Wall -Wextra -Werror -Wpedantic)
if(ENABLE_DEBUG)
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -g
)
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
