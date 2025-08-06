#include "Framer.hpp"
#include <iostream>
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

int runRenderer(const ObjParser& parser, const std::string& windowTitle) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW.\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, windowTitle.c_str(), nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window.\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD.\n";
        glfwTerminate();
        return -1;
    }

    Mesh mesh(parser.vertices, parser.edges);
    Shader shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");

    ModelMatrix modelMatrix;
    modelMatrix.setTranslation(glm::vec3(0.0f, 0.0f, 0.0f));
    modelMatrix.setRotation(0.0f, glm::vec3(0,1,0));
    modelMatrix.setScale(glm::vec3(1.0f,1.0f,1.0f));

    ViewMatrix viewMatrix;
    viewMatrix.setCamera(glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(0,1,0));

    ProjectionMatrix projectionMatrix;
    float aspect = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);
    projectionMatrix.setPerspective(45.0f, aspect, 0.1f, 100.0f);

    Renderer renderer(window, mesh, shader, modelMatrix, viewMatrix, projectionMatrix);
    glfwSetWindowUserPointer(window, &renderer);

    while (!glfwWindowShouldClose(window)) {
        renderer.renderFrame();
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    void* ptr = glfwGetWindowUserPointer(window);
    if (ptr) {
        static_cast<Renderer*>(ptr)->onWindowResize(width, height);
    } else {
        glViewport(0, 0, width, height);
    }
}

