cmake_minimum_required(VERSION 3.10)
project(opengl_sandbox)

# Global settings

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#   Find packages

find_package(glad)
find_package(glfw3)

# Example subdirectories

add_subdirectory(examples/hello_window)