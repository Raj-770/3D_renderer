#pragma once

#include <glm/glm.hpp>

class ProjectionMatrix {
public:
    enum class Type {
        Perspective,
        Orthographic
    };

    ProjectionMatrix();

    void setPerspective(float fovDegrees, float aspect, float nearPlane, float farPlane);

    void setOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane);

    glm::mat4 getMatrix() const;

    Type getType() const;

private:
    glm::mat4 matrix_;
    Type type_;
};
