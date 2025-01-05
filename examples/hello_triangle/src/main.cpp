#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "main.hpp"
#include <iostream>


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Vertex input - Triangle
// X, Y, Z (depth)

float vertices[] ={
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};

int main() {

}

// vertex buffer object
// can send large batchs of data at once (like a buffer)
unsigned int VBO;
glGenBuffer(1, &VBO);

