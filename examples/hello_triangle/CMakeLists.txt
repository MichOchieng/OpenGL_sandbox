add_executable(hello_triangle
    src/main.cpp
)

target_link_libraries(hello_triangle
    PRIVATE
        glad::glad
        glfw
        common
)