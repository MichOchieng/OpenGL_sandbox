cmake_minimum_required(VERSION 3.10)
project(opengl_sandbox)

# Global settings

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#   Find packages

find_package(glad)
find_package(glfw3)
find_package(glm)
find_package(assimp)

# Libraries
add_library(common
    common/src/window/window.cpp
    common/src/model_loader/model_loader.cpp
    common/src/camera/camera.cpp
)

target_include_directories(common
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/common/include
)

target_link_libraries(common
    PUBLIC
        glad::glad
        glfw
        glm::glm
        assimp::assimp
)

# Example subdirectories

add_subdirectory(examples/hello_window)
add_subdirectory(examples/hello_triangle)

add_subdirectory(examples/shaders)
add_subdirectory(examples/textures)
add_subdirectory(examples/camera)

add_subdirectory(examples/models)
