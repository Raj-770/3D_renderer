#include "Framer.hpp"
#include <glm/gtc/matrix_transform.hpp>

InputHandler::InputHandler(GLFWwindow* window, ViewMatrix& viewMatrix)
    : window_(window), viewMatrix_(viewMatrix), lastX_(0), lastY_(0), leftMousePressed_(false)
{}

void InputHandler::registerCallbacks() {
    glfwSetCursorPosCallback(window_, cursorPosCallback);
    glfwSetMouseButtonCallback(window_, mouseButtonCallback);
    glfwSetScrollCallback(window_, scrollCallback);
}

static AppContext* getAppContext(GLFWwindow* window) {
    return static_cast<AppContext*>(glfwGetWindowUserPointer(window));
}

void InputHandler::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    AppContext* ctx = getAppContext(window);
    if (ctx && ctx->inputHandler) {
        ctx->inputHandler->handleCursorPos(xpos, ypos);
    }
}

void InputHandler::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    AppContext* ctx = getAppContext(window);
    if (ctx && ctx->inputHandler) {
        ctx->inputHandler->handleMouseButton(button, action, mods);
    }
}

void InputHandler::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    AppContext* ctx = getAppContext(window);
    if (ctx && ctx->inputHandler) {
        ctx->inputHandler->handleScroll(xoffset, yoffset);
    }
}

void InputHandler::handleCursorPos(double xpos, double ypos) {
    if (!leftMousePressed_) {
        lastX_ = xpos;
        lastY_ = ypos;
        return;
    }
    float xoffset = static_cast<float>(xpos - lastX_);
    float yoffset = static_cast<float>(ypos - lastY_);
    lastX_ = xpos; lastY_ = ypos;

    float sensitivity = 0.25f;
    yaw += xoffset * sensitivity;
    pitch += yoffset * sensitivity;
    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 pos;
    pos.x = target.x + distance * cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    pos.y = target.y + distance * sin(glm::radians(pitch));
    pos.z = target.z + distance * sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    viewMatrix_.setCamera(pos, target, glm::vec3(0, 1, 0));
}

void InputHandler::handleMouseButton(int button, int action, int /*mods*/) {
    if (button == GLFW_MOUSE_BUTTON_LEFT)
        leftMousePressed_ = (action == GLFW_PRESS);
}

void InputHandler::handleScroll(double /*xoffset*/, double yoffset) {
    float zoomSensitivity = 0.1f;
    distance -= static_cast<float>(yoffset) * zoomSensitivity;
    if (distance < 1.0f) distance = 1.0f;
    if (distance > 100.0f) distance = 100.0f;

    float radYaw = glm::radians(yaw), radPitch = glm::radians(pitch);
    glm::vec3 pos;
    pos.x = target.x + distance * cos(radYaw) * cos(radPitch);
    pos.y = target.y + distance * sin(radPitch);
    pos.z = target.z + distance * sin(radYaw) * cos(radPitch);
    viewMatrix_.setCamera(pos, target, glm::vec3(0, 1, 0));
}
