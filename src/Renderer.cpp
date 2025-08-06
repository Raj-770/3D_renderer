#include "Renderer.hpp"
#include <iostream>

namespace {
    constexpr int default_width = 800;
    constexpr int default_height = 600;
}

Renderer::Renderer(GLFWwindow* window,
                   Mesh& mesh,
                   Shader& shader,
                   ModelMatrix& model,
                   ViewMatrix& view,
                   ProjectionMatrix& projection)
    : window_(window), mesh_(mesh), shader_(shader), model_(model), view_(view), projection_(projection),
      windowWidth_(default_width), windowHeight_(default_height)
{
    // Enable depth test for correct rendering order
    glEnable(GL_DEPTH_TEST);

    // Set wireframe mode (for mesh lines)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Get initial window size and set viewport
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    windowWidth_ = width;
    windowHeight_ = height;
    glViewport(0, 0, windowWidth_, windowHeight_);

    updateProjectionMatrix();
}

Renderer::~Renderer() {
    // Mesh and Shader cleanup handled by their destructors
}

void Renderer::renderFrame() {
    // Clear screen to dark gray, with depth buffer
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader_.use();

    // Pass transformation matrices to GPU
    shader_.setMat4("model", model_.getModelMatrix());
    shader_.setMat4("view", view_.getViewMatrix());
    shader_.setMat4("projection", projection_.getMatrix());

    // Set wireframe color uniform (white)
    shader_.setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));

    // Draw the wireframe mesh
    mesh_.draw();

    // Present the result and process events
    glfwSwapBuffers(window_);
    glfwPollEvents();
}

void Renderer::onWindowResize(int width, int height) {
    if (width == 0 || height == 0) return;
    windowWidth_ = width;
    windowHeight_ = height;
    glViewport(0, 0, windowWidth_, windowHeight_);
    updateProjectionMatrix();
}

void Renderer::updateProjectionMatrix() {
    using Type = ProjectionMatrix::Type;
    if (projection_.getType() == Type::Perspective) {
        float aspectRatio = static_cast<float>(windowWidth_) / static_cast<float>(windowHeight_);
        // Update with new aspect ratio (defaults: FOV 45, near 0.1, far 100)
        projection_.setPerspective(45.0f, aspectRatio, 0.1f, 100.0f);
    }
    // For orthographic projection: supply updated parameters or handle aspect as needed
}
