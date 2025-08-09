#pragma once

#include "MiniGLM.hpp" 

class ViewMatrix {
public:
    ViewMatrix();

    void setCamera(const MiniGLM::vec3& position, const MiniGLM::vec3& target, const MiniGLM::vec3& up);

    void setPosition(const MiniGLM::vec3& position);
    void setTarget(const MiniGLM::vec3& target);
    void setUp(const MiniGLM::vec3& up);

    MiniGLM::vec3 getPosition() const;
    MiniGLM::vec3 getTarget() const;
    MiniGLM::vec3 getUp() const;

    MiniGLM::mat4 getViewMatrix() const;

private:
    MiniGLM::vec3 position_;
    MiniGLM::vec3 target_;
    MiniGLM::vec3 up_;

    mutable MiniGLM::mat4 view_; 
    mutable bool dirty_;

    void updateViewMatrix() const;
};
