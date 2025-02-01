
add_executable(textures
    src/main.cpp
)

target_link_libraries(textures
    PRIVATE
        glad::glad
        glfw
        common
)
