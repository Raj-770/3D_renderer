#pragma once
#include <GLFW/glfw3.h>
#include "ViewMatrix.hpp"

class InputHandler {
public:
    InputHandler(GLFWwindow* window, ViewMatrix& viewMatrix);

    void registerCallbacks();

    void processInput();

    float yaw = 0.0f;
    float pitch = 0.0f;
    float distance = 10.0f;
    glm::vec3 target = glm::vec3(0,0,0);

private:
    GLFWwindow* window_;
    ViewMatrix& viewMatrix_;

    double lastX_, lastY_;
    bool leftMousePressed_;

    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    void handleCursorPos(double xpos, double ypos);
    void handleMouseButton(int button, int action, int mods);
    void handleScroll(double xoffset, double yoffset);
};
