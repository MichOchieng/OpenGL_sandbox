#pragma once // ensures files included once in compilation units

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

class Window {
    public:
        // Default params
        // width: 800px
        // height: 600px
        // title: OpenGL Sandbox

        Window(
            int width = 800,
            int height = 600,
            const std::string& title = "OpenGL Sandbox"
        );

        ~Window(); // clean up resources

        bool shouldClose() const;
        void swapBuffers();
        void pollEvents();
        void processInput();

        GLFWwindow* getWindow() { return window; }

    private:
        GLFWwindow* window;
        int width;
        int height;
        std::string title;

        // callback for window resizing events
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

        // init helpers
        bool initGLFW(); // setup GLFW w/ OpenGL context settings
        bool initGLAD(); // load OpenGl function pointers
}