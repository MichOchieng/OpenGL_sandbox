add_executable(hello_window
    src/main.cpp
    src/window/window.cpp    
)

target_link_libraries(hello_window
    PRIVATE
        glad::glad
        glfw
)