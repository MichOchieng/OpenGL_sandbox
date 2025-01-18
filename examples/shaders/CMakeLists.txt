
add_executable(shaders
    src/main.cpp
)

target_link_libraries(shaders
    PRIVATE
        glad::glad
        glfw
        common
)
