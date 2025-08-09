#pragma once

#include <vector>
#include "MiniGLM.hpp" // Your replacement for GLM

class ScreenMapper {
public:
    ScreenMapper(int screenWidth, int screenHeight);

    void setScreenSize(int width, int height);

    // Converts transformed vertices (vec4 in clip space) to pixel positions
    std::vector<MiniGLM::ivec2> mapToScreen(const std::vector<MiniGLM::vec4>& clipSpaceVertices) const;

private:
    int screenWidth_;
    int screenHeight_;
};
