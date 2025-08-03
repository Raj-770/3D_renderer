#include "ProjectionMatrix.hpp"
#include <glm/gtc/matrix_transform.hpp>

ProjectionMatrix::ProjectionMatrix()
    : matrix_(1.0f), type_(Type::Perspective) {}

void ProjectionMatrix::setPerspective(float fovDegrees, float aspect, float nearPlane, float farPlane) {
    matrix_ = glm::perspective(glm::radians(fovDegrees), aspect, nearPlane, farPlane);
    type_ = Type::Perspective;
}

void ProjectionMatrix::setOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
    matrix_ = glm::ortho(left, right, bottom, top, nearPlane, farPlane);
    type_ = Type::Orthographic;
}

glm::mat4 ProjectionMatrix::getMatrix() const {
    return matrix_;
}

ProjectionMatrix::Type ProjectionMatrix::getType() const {
    return type_;
}
