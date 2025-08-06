#include "Renderer.hpp"
#include <iostream>

/**
 * @class Renderer
 * 
 * @brief Handles the rendering loop and manages OpenGL context setup for mesh visualiztion.
 * 
 *        The Renderer class encapsulates the OpenGl rendering logic, window management and 
 *        passes transformation matrices and uniforms to the shader program for each frame.
 *        
 *        It supports dynamic handling of windoe resizing, sets OpenGL state for wireframe
 *        rendering, and links together the key objects for drawing: Mesh, Shader, ModelMatrix,
 *        ViewMatrix and ProjectionMatrix.
 * 
 */
namespace {
    constexpr int default_width = 800;
    constexpr int default_height = 600;
}

/**
 * @brief Construct a Renderer and initializes the OpenGL context.
 *        
 *        Initializes OpenGL state, queries the inital window size, configures the viewport,
 *        and sets up the projection matrix.
 * 
 * @param window Pointer to the GLFWwindow
 * @param mesh Refrence to the Mesh to render
 * @param shader Refrence to the Shader program for rendering
 * @param model Reference to ModelMatrix for object transformation
 * @param view view Reference to ViewMatrix for camera transforms
 * @param projection projection Reference to ProjectionMatrix for projection
 */
Renderer::Renderer(GLFWwindow* window,
                   Mesh& mesh,
                   Shader& shader,
                   ModelMatrix& model,
                   ViewMatrix& view,
                   ProjectionMatrix& projection)
    : window_(window), mesh_(mesh), shader_(shader), model_(model), view_(view), projection_(projection),
      windowWidth_(default_width), windowHeight_(default_height)
{
    glEnable(GL_DEPTH_TEST);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    windowWidth_ = width;
    windowHeight_ = height;
    glViewport(0, 0, windowWidth_, windowHeight_);

    updateProjectionMatrix();
}

/**
 * @brief Destructor; any resource cleanup is delegated to other objects.
 */
Renderer::~Renderer() {
    // Mesh and Shader cleanup handled by their destructors
}

/**
 * @brief Renders a single frame.
 *
 * Clears the color and depth buffers, activates the shader, uploads model, view, and projection matrices,
 * sets the mesh wireframe color, draws the mesh, swaps the display buffers, and polls for window events.
 */
void Renderer::renderFrame() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    shader_.use();
    shader_.setMat4("model", model_.getModelMatrix());
    shader_.setMat4("view", view_.getViewMatrix());
    shader_.setMat4("projection", projection_.getMatrix());
    shader_.setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
    mesh_.draw();
    glfwSwapBuffers(window_);
    glfwPollEvents();
}

/**
 * @brief Handles window resize events.
 *
 * Updates internal width and height, adjusts the OpenGL viewport, and recalculates the projection matrix.
 *
 * @param width New window width in pixels.
 * @param height New window height in pixels.
 */
void Renderer::onWindowResize(int width, int height) {
    if (width == 0 || height == 0) return;
    windowWidth_ = width;
    windowHeight_ = height;
    glViewport(0, 0, windowWidth_, windowHeight_);
    updateProjectionMatrix();
}

/**
 * @brief Updates the projection matrix parameters based on the current viewport size.
 *
 * For a perspective projection, recalculates the aspect ratio and updates the matrix.
 * 
 * @todo need to extend it for orthogrpahic projection
 */
void Renderer::updateProjectionMatrix() {
    using Type = ProjectionMatrix::Type;
    if (projection_.getType() == Type::Perspective) {
        float aspectRatio = static_cast<float>(windowWidth_) / static_cast<float>(windowHeight_);
        projection_.setPerspective(45.0f, aspectRatio, 0.1f, 100.0f);
    }
    // For orthographic projection: supply updated parameters or handle aspect as needed
}
