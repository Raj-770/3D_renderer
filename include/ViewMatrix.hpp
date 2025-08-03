#pragma once

#include <glm/glm.hpp>

class ViewMatrix {
public:
    ViewMatrix();

    void setCamera(const glm::vec3& position, const glm::vec3& target, const glm::vec3& up);

    void setPosition(const glm::vec3& position);
    void setTarget(const glm::vec3& target);
    void setUp(const glm::vec3& up);

    glm::vec3 getPosition() const;
    glm::vec3 getTarget() const;
    glm::vec3 getUp() const;

    glm::mat4 getViewMatrix() const;

private:
    glm::vec3 position_;
    glm::vec3 target_;
    glm::vec3 up_;

    mutable glm::mat4 view_; 
    mutable bool dirty_;

    void updateViewMatrix() const;
};
