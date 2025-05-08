
add_executable(models
    src/main.cpp
    src/modelLoading.cpp
)

target_link_libraries(models
    PRIVATE
        glad::glad
        glfw
        common
        assimp::assimp
)
