#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

class Camera {
    private:
        
        glm::vec3 position;

        float yaw;
        float pitch;
        float speed;
        float fov;
        float nearPlane;
        float farPlane;

        float deltaTime;

        float mouseSensitivity;

        // Inputs
        bool moveUp, moveDown, moveLeft, moveRight, moveForward, moveBackward;

        double prevXPos;
        double prevYPos;

        const float TWO_PI = 2.0f * 3.14159265359f;

    public:

        Camera(const glm::vec3& startPosition = glm::vec3(0.0f));
        ~Camera();
        
        // Register with GLFW window
        void registerCallbacks(GLFWwindow* window);
        
        // Input handling
        void processKeyInput(int key, int action);
        void processMouseMovement(double xpos, double ypos);
        
        // Movement processing
        void update(float dt);
        
        // Getters for matrices
        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix(int screenWidth, int screenHeight) const;
        
        // Getters/Setters
        glm::vec3 getPosition() const { return position; }
        glm::vec3 getLookVector() const;
        void setPosition(const glm::vec3& pos) { position = pos; }
        void setFOV(float newFov) { fov = newFov; }
        float getFOV() const { return fov; }
        
        // Input state setters
        void setMoveForward(bool state) { moveForward = state; }
        void setMoveBackward(bool state) { moveBackward = state; }
        void setMoveLeft(bool state) { moveLeft = state; }
        void setMoveRight(bool state) { moveRight = state; }
        void setMoveUp(bool state) { moveUp = state; }
        void setMoveDown(bool state) { moveDown = state; }
};
