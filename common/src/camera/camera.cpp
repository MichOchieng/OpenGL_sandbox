#include "camera/camera.hpp"
#include <iostream>

// Static callback functions
static Camera* activeCamera = nullptr;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (activeCamera) {
        activeCamera->processKeyInput(key, action);
    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (activeCamera) {
        activeCamera->processMouseMovement(xpos, ypos);
    }
}

Camera::Camera(const glm::vec3& startPosition)
    : position(startPosition),
      yaw(0.0f),
      pitch(0.0f),
      speed(10.0f),
      mouseSensitivity(0.001f),
      fov(90.0f),
      nearPlane(0.1f),
      farPlane(100.0f),
      deltaTime(0.0f),
      moveForward(false),
      moveBackward(false),
      moveLeft(false),
      moveRight(false),
      moveUp(false),
      moveDown(false),
      prevXPos(0.0),
      prevYPos(0.0)
{
}

Camera::~Camera() {
    // If this camera is the active one, reset the global pointer
    if (activeCamera == this) {
        activeCamera = nullptr;
    }
}

void Camera::registerCallbacks(GLFWwindow* window) {
    // Set this camera as the active one for callbacks
    activeCamera = this;
    
    // Register the callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    
    // Initialize cursor position to avoid jumps on first movement
    glfwGetCursorPos(window, &prevXPos, &prevYPos);
}

void Camera::processKeyInput(int key, int action) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
    if (key == GLFW_KEY_W)
        moveForward = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_S)
        moveBackward = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_A)
        moveLeft = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_D)
        moveRight = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_SPACE)
        moveUp = (action != GLFW_RELEASE);
    if (key == GLFW_KEY_LEFT_SHIFT)
        moveDown = (action != GLFW_RELEASE);
}

void Camera::processMouseMovement(double xpos, double ypos) {
    float xoffset = (xpos - prevXPos) * mouseSensitivity;
    float yoffset = (prevYPos - ypos) * mouseSensitivity; // reversed since y-coordinates go from bottom to top
    
    yaw += xoffset;
    pitch += yoffset;
    
    prevXPos = xpos;
    prevYPos = ypos;
}

glm::vec3 Camera::getLookVector() const {
    glm::vec3 forward;
    forward.x = cos(pitch * TWO_PI) * cos(yaw * TWO_PI);
    forward.y = sin(pitch * TWO_PI);
    forward.z = cos(pitch * TWO_PI) * sin(yaw * TWO_PI);
    return glm::normalize(forward);
}

void Camera::update(float dt) {
    deltaTime = dt;
    
    glm::vec3 up_vec = glm::vec3(0.0f, 1.0f, 0.0f);
    
    // Compute forward and right vectors
    glm::vec3 forward_vec = getLookVector();
    glm::vec3 right_vec = glm::normalize(glm::cross(forward_vec, up_vec));
    
    glm::vec3 movement(0.0f);
    
    if (moveForward)
        movement += forward_vec;
    if (moveBackward)
        movement -= forward_vec;
    if (moveLeft)
        movement -= right_vec;
    if (moveRight)
        movement += right_vec;
    if (moveUp)
        movement += up_vec;
    if (moveDown)
        movement -= up_vec;
    
    if (glm::length(movement) > 0.0f) {
        movement = glm::normalize(movement);
    }
    
    position += movement * speed * deltaTime;
}

glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, position + getLookVector(),
                      glm::vec3(0.0f, 1.0f, 0.0f));
}

glm::mat4 Camera::getProjectionMatrix(int screenWidth, int screenHeight) const {
    return glm::perspective(glm::radians(fov),
                           static_cast<float>(screenWidth) / static_cast<float>(screenHeight),
                           nearPlane, farPlane);
}
