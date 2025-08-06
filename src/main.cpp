#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ObjParser.hpp"
#include "Mesh.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

// Forward declare resize callback
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// Entry point
int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: ./framer <.obj file>\n";
        return 1;
    }

    // 1. Parse OBJ file
    ObjParser parser;
    if (!parser.load(argv[1])) {
        std::cerr << "Failed to load OBJ file.\n";
        return 1;
    }
    std::cout << "Loaded " << parser.vertices.size() << " vertices and " 
              << parser.faces.size() << " faces with " 
              << parser.edges.size() << " edges.\n";

    // 2. GLFW init
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW.\n";
        return -1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "3D Wireframe Renderer", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window.\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // 3. GLAD init
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD.\n";
        glfwTerminate();
        return -1;
    }

    // 4. Prepare core renderer objects
    Mesh mesh(parser.vertices, parser.edges);

    Shader shader("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");

    ModelMatrix modelMatrix;
    modelMatrix.setTranslation(glm::vec3(0.0f, 0.0f, 0.0f));
    modelMatrix.setRotation(0.0f, glm::vec3(0, 1, 0));
    modelMatrix.setScale(glm::vec3(1.0f, 1.0f, 1.0f));

    ViewMatrix viewMatrix;
    viewMatrix.setCamera(glm::vec3(0.0f, 0.0f, 10.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    ProjectionMatrix projectionMatrix;
    float aspect = static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT);
    projectionMatrix.setPerspective(45.0f, aspect, 0.1f, 100.0f);

    Renderer renderer(window, mesh, shader, modelMatrix, viewMatrix, projectionMatrix);

    // Set the renderer as user pointer for using in callback
    glfwSetWindowUserPointer(window, &renderer);

    // 5. Main render loop
    while (!glfwWindowShouldClose(window)) {
        renderer.renderFrame();
        // Insert custom input/camera controls here if desired
    }

    // 6. Cleanup and exit
    glfwTerminate();
    return 0;
}

// Framebuffer size callback, so resizing the window updates the viewport/projection
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    void* ptr = glfwGetWindowUserPointer(window);
    if (ptr) {
        static_cast<Renderer*>(ptr)->onWindowResize(width, height);
    } else {
        glViewport(0, 0, width, height);
    }
}
