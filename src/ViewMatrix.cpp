#include "ViewMatrix.hpp"
#include <glm/gtc/matrix_transform.hpp>

ViewMatrix::ViewMatrix()
    : position_(0.0f, 0.0f, 10.0f),
      target_(0.0f, 0.0f, 0.0f),
      up_(0.0f, 1.0f, 0.0f),
      dirty_(true)
{}

void ViewMatrix::setCamera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up) {
    position_ = position;
    target_ = target;
    up_ = up;
    dirty_ = true;
}

void ViewMatrix::setPosition(const glm::vec3& position) {
    position_ = position;
    dirty_ = true;
}

void ViewMatrix::setTarget(const glm::vec3& target) {
    target_ = target;
    dirty_ = true;
}

void ViewMatrix::setUp(const glm::vec3& up) {
    up_ = up;
    dirty_ = true;
}

glm::vec3 ViewMatrix::getPosition() const { return position_; }
glm::vec3 ViewMatrix::getTarget() const { return target_; }
glm::vec3 ViewMatrix::getUp() const { return up_; }

glm::mat4 ViewMatrix::getViewMatrix() const {
    if (dirty_) {
        updateViewMatrix();
        dirty_ = false;
    }
    return view_;
}

void ViewMatrix::updateViewMatrix() const {
    view_ = glm::lookAt(position_, target_, up_);
}
