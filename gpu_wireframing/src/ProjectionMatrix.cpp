#include "ProjectionMatrix.hpp"


/**
 * @brief Construct a new Projection Matrix:: Projection Matrix object
 *        Manages 3D projection matrices (perspective and orthographic) for rendering pipeline
 *        - Initializes to identity matrix
 *        - Default projection is perspective 
 */
ProjectionMatrix::ProjectionMatrix()
    : matrix_(1.0f), type_(Type::Perspective) {}

/**
 * @brief Sets the matrix to perspective projection
 * 
 * @param fovDegrees Field of view in degrees
 * @param aspect Aspect ratio (width/height) of the viewing window
 * @param nearPlane Distance to the near clipping plane
 * @param farPlane Distance to the far clipping plane
 */
void ProjectionMatrix::setPerspective(float fovDegrees, float aspect, float nearPlane, float farPlane) {
    matrix_ = MiniGLM::perspective(MiniGLM::radians(fovDegrees), aspect, nearPlane, farPlane);
    type_ = Type::Perspective;
}

/**
 * @brief Sets the matrix to an orthographic projection
 * 
 * @param left Left clipping plane
 * @param right Right clipping plane
 * @param bottom Bottom clipping plane
 * @param top Top clipping plane
 * @param nearPlane Near clipping plane
 * @param farPlane Far clipping plane
 */
void ProjectionMatrix::setOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
    matrix_ = MiniGLM::ortho(left, right, bottom, top, nearPlane, farPlane);
    type_ = Type::Orthographic;
}

/**
 * @brief Getter for the current porjection matrix
 * 
 * @return MiniGLMmat4 
 */
MiniGLM::mat4 ProjectionMatrix::getMatrix() const {
    return matrix_;
}

/**
 * @brief Getter for the current projection type (Perspective or Orthographic)
 * 
 * @return ProjectionMatrix::Type 
 */
ProjectionMatrix::Type ProjectionMatrix::getType() const {
    return type_;
}
