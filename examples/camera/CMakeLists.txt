
add_executable(camera
    src/main.cpp
)

target_link_libraries(camera
    PRIVATE
        glad::glad
        glfw
        common
)
