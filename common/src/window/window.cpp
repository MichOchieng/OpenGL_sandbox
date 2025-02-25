#include "window/window.hpp"
#include <iostream>

Window::Window(int width, int height, const std::string &title, bool isFullscreen) : width(width), height(height), title(title), window(nullptr)
{

    if (!initGLFW())
    {
        throw std::runtime_error("Failed to initialize GLFW!");
    }

    if (isFullscreen)
    {
        window = glfwCreateWindow(width, height, title.c_str(), glfwGetPrimaryMonitor(), nullptr);
    }
    else
    {
        window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
    }

    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window!");
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!initGLAD())
    {
        throw std::runtime_error("Failed to initialize GLAD!");
    }
}

Window::~Window()
{
    glfwTerminate();
}

bool Window::initGLFW()
{
    if (!glfwInit())
    {
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    return true;
}

bool Window::initGLAD() {
    return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

void Window::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

bool Window::shouldClose() const {
    return glfwWindowShouldClose(window);
}

void Window::swapBuffers() {
    glfwSwapBuffers(window);
}

void Window::pollEvents() {
    glfwPollEvents();
}

void Window::processInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}