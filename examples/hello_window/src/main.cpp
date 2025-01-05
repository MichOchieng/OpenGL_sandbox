#include "window/window.hpp"
#include <iostream>

int main() {
    try {
        Window window(800, 600, "Example 1: Hello Window");

        while (!window.shouldClose()) {
            window.processInput();
            window.swapBuffers();
            window.pollEvents();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}