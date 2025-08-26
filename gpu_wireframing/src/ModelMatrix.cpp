#include "ModelMatrix.hpp"

/**
 * @brief Construct a new Model Matrix:: Model Matrix object which Handles
 * transformation and model matrix genertion for 3d models.
 *        - Initialize translation to origin
 *        - No initial rotation
 *        - Initial rotation axis is Y axis
 *        - Uniform scaling
 */
ModelMatrix::ModelMatrix()
    : translation_(0.0f), rotationAngle_(0.0f), rotationAxis_(0.0f, 1.0f, 0.0f),
      scale_(1.0f), dirty_(true) {}

/**
 * @brief Sets the translation vector for the model
 *
 * @param translation New translation vector (position in world space)
 */
void ModelMatrix::setTranslation(const MiniGLM::vec3 &translation) {
  translation_ = translation;
  dirty_ = true;
}

/**
 * @brief Sets the roation for the model
 *
 * @param angleDegrees Rotation angle in degrees.
 * @param axis Axis to rotate around (must be normalized)
 */
void ModelMatrix::setRotation(float angleDegrees, const MiniGLM::vec3 &axis) {
  rotationAngle_ = angleDegrees;
  rotationAxis_ = axis;
  dirty_ = true;
}

/**
 * @brief Sets the scale vector for the model
 *
 * @param scale New scale fractors for x, y, z axes
 */
void ModelMatrix::setScale(const MiniGLM::vec3 &scale) {
  scale_ = scale;
  dirty_ = true;
}

/**
 * @brief Returns the composed model matrix after applying translation,
 * rotation, and scale Updates the matrix if any transformation has changed
 * since last computation.
 * @return MiniGLMmat4
 */
MiniGLM::mat4 ModelMatrix::getModelMatrix() const {
  if (dirty_) {
    updateModelMatrix();
    dirty_ = false;
  }
  return model_;
}

/**
 * @brief Recalculates the model matrix from translation, rotation and scale.
 *        Applies tranformations in order: translate, rotate, then scale.
 *
 */
void ModelMatrix::updateModelMatrix() const {
  MiniGLM::mat4 mat = MiniGLM::mat4(1.0f);
  mat = MiniGLM::translate(mat, translation_);
  mat = MiniGLM::rotate(mat, MiniGLM::radians(rotationAngle_), rotationAxis_);
  mat = MiniGLM::scale(mat, scale_);
  model_ = mat;
}
