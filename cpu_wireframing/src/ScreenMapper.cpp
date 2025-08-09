#include "ScreenMapper.hpp"
#include <algorithm>

ScreenMapper::ScreenMapper(int screenWidth, int screenHeight)
    : screenWidth_(screenWidth), screenHeight_(screenHeight)
{}

void ScreenMapper::setScreenSize(int width, int height) {
    screenWidth_ = width;
    screenHeight_ = height;
}

// Assumes input is already perspective-divided (clip.xy/clip.w); does it if not
std::vector<MiniGLM::ivec2> ScreenMapper::mapToScreen(const std::vector<MiniGLM::vec4>& clipSpaceVertices) const
{
    std::vector<MiniGLM::ivec2> screenVerts;
    screenVerts.reserve(clipSpaceVertices.size());
    for(const auto& v : clipSpaceVertices) {
        // Perspective divide
        float ndc_x = v.x / v.w;
        float ndc_y = v.y / v.w;

        // Map normalized device coords [-1,1] → [0,W] and [0,H]
        int px = static_cast<int>((ndc_x * 0.5f + 0.5f) * screenWidth_);
        int py = static_cast<int>((1.0f - (ndc_y * 0.5f + 0.5f)) * screenHeight_); // y-flip for top-left origin

        // Optionally clamp
        px = std::clamp(px, 0, screenWidth_ - 1);
        py = std::clamp(py, 0, screenHeight_ - 1);

        screenVerts.emplace_back(MiniGLM::ivec2(px, py));
    }
    return screenVerts;
}
