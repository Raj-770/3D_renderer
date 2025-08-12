#pragma once

#include <vector>
#include "MiniGLM.hpp"

class ScreenMapper {
public:
    ScreenMapper(int screenWidth, int screenHeight);

    void setScreenSize(int width, int height);

    std::vector<MiniGLM::ivec2> mapToScreen(const std::vector<MiniGLM::vec4>& clipSpaceVertices) const;

private:
    int screenWidth_;
    int screenHeight_;
};
