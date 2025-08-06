#include "ViewMatrix.hpp"
#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief Construct a new View Matrix:: View Matrix object
 *        Manages camera postition, orientation and view matrix generation
 *        - Default Camera Position at (0, 0, 10)
 *        - Default camera looking toward the origin
 *        - Default Up vector along Y axis
 */
ViewMatrix::ViewMatrix()
    : position_(0.0f, 0.0f, 10.0f),
      target_(0.0f, 0.0f, 0.0f),
      up_(0.0f, 1.0f, 0.0f),
      dirty_(true)
{}

/**
 * @brief Sets the camera's position, target and up vector
 * 
 * @param position Camera's postition in world coordinates
 * @param target Point the camera is looking at
 * @param up Up direction for the camera
 */
void ViewMatrix::setCamera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up) {
    position_ = position;
    target_ = target;
    up_ = up;
    dirty_ = true;
}

/**
 * @brief Sets the cameras position
 * 
 * @param position 
 */
void ViewMatrix::setPosition(const glm::vec3& position) {
    position_ = position;
    dirty_ = true;
}

/**
 * @brief Sets the cameras target point
 * 
 * @param target 
 */
void ViewMatrix::setTarget(const glm::vec3& target) {
    target_ = target;
    dirty_ = true;
}

/**
 * @brief Sets the camera's up vector
 * 
 * @param up 
 */
void ViewMatrix::setUp(const glm::vec3& up) {
    up_ = up;
    dirty_ = true;
}

/*
 * Getters for different camera variables
 */
glm::vec3 ViewMatrix::getPosition() const { return position_; }
glm::vec3 ViewMatrix::getTarget() const { return target_; }
glm::vec3 ViewMatrix::getUp() const { return up_; }

/**
 * @brief Returns the camera's view matrix, recalculation if camera state has changes
 * 
 * @return glm::mat4 
 */
glm::mat4 ViewMatrix::getViewMatrix() const {
    if (dirty_) {
        updateViewMatrix();
        dirty_ = false;
    }
    return view_;
}

/**
 * @brief Recomputes the view matrix using lookAt with current camera parameters.
 *
 * Applies glm::lookAt to construct the view matrix from the camera's positon, target and up vector
 */
void ViewMatrix::updateViewMatrix() const {
    view_ = glm::lookAt(position_, target_, up_);
}
