#include "ModelMatrix.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

ModelMatrix::ModelMatrix()
    : translation_(0.0f), rotationAngle_(0.0f), rotationAxis_(0.0f, 1.0f, 0.0f), scale_(1.0f), dirty_(true) {}

void ModelMatrix::setTranslation(const glm::vec3& translation) {
    translation_ = translation;
    dirty_ = true;
}
void ModelMatrix::setRotation(float angleDegrees, const glm::vec3& axis) {
    rotationAngle_ = angleDegrees;
    rotationAxis_ = axis;
    dirty_ = true;
}
void ModelMatrix::setScale(const glm::vec3& scale) {
    scale_ = scale;
    dirty_ = true;
}

glm::mat4 ModelMatrix::getModelMatrix() const {
    if (dirty_) {
        updateModelMatrix();
        dirty_ = false;
    }
    return model_;
}

void ModelMatrix::updateModelMatrix() const {
    glm::mat4 mat = glm::mat4(1.0f);
    mat = glm::translate(mat, translation_);
    mat = glm::rotate(mat, glm::radians(rotationAngle_), rotationAxis_);
    mat = glm::scale(mat, scale_);
    model_ = mat;
}
