add_executable(hello_window
    src/main.cpp 
)

target_link_libraries(hello_window
    PRIVATE
        glad::glad
        glfw
        common
)