#include "Framer.hpp"
#include <iostream>
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "InputHandler.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

/**
 * @brief Entry point and render loop for the 3D wireframe viewer application.
 *
 * This file contains setup routines for the OpenGL context, GLFW initialization, mesh loading,
 * shader compilation, and the rendering main loop. It connects all key graphics components and
 * allows for dynamic window resizing.
 */


const unsigned int WINDOW_WIDTH = 1200;
const unsigned int WINDOW_HEIGHT = 800;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

/**
 * @brief Initializes OpenGL, loads scene data, sets up rendering objects, and runs the main render loop.
 *
 * Creates the OpenGL context and window, loads vertex and edge data from the provided ObjParser,
 * compiles shaders, constructs for the mesh, transformation matrices, and renderer. Handles
 * window resizing and entire application lifecycle.
 *
 * @param parser Parsed OBJ mesh source containing vertices and edges.
 * @param windowTitle The title for the application window.
 * @return Exit code: 0 on success, negative on error.
 */
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
    Shader shader("shaders/advanced/vertex_shader.glsl", "shaders/advanced/fragment_shader.glsl");

    ModelMatrix modelMatrix;
    modelMatrix.setTranslation(glm::vec3(0.0f, 0.0f, 0.0f));
    modelMatrix.setRotation(0.0f, glm::vec3(0,1,0));
    modelMatrix.setScale(glm::vec3(0.1f,0.1f,0.1f));

    ViewMatrix viewMatrix;
    viewMatrix.setCamera(glm::vec3(0,0,10), glm::vec3(0,0,0), glm::vec3(0,1,0));

    InputHandler inputHandler(window, viewMatrix);
    inputHandler.registerCallbacks();

    ProjectionMatrix projectionMatrix;
    float aspect = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);
    projectionMatrix.setPerspective(45.0f, aspect, 0.1f, 100.0f);

    Renderer renderer(window, mesh, shader, modelMatrix, viewMatrix, projectionMatrix);
    AppContext context{&renderer, &inputHandler};
    glfwSetWindowUserPointer(window, &context);

    while (!glfwWindowShouldClose(window)) {
        renderer.renderFrame();
    }

    glfwTerminate();
    return 0;
}

/**
 * @brief GLFW callback for updating the viewport and renderer state on window resize.
 *
 * @param window Pointer to the GLFW window.
 * @param width New framebuffer width.
 * @param height New framebuffer height.
 */
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    void* ptr = glfwGetWindowUserPointer(window);
    if (ptr) {
        auto context = static_cast<AppContext*>(ptr);
        if (context->renderer) {
            context->renderer->onWindowResize(width, height);
        }
    } else {
        glViewport(0, 0, width, height);
    }
}

