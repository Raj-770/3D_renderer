#include "ScreenMapper.hpp"
#include <algorithm>

/**
 * @brief Constructs a ScreenMapper with specified screen dimensions.
 * 
 * Initializes the mapping context that converts normalized device coordinates (NDC)
 * to pixel coordinates on a display of given width and height.
 * 
 * @param screenWidth The width of the screen in pixels.
 * @param screenHeight The height of the screen in pixels.
 */
ScreenMapper::ScreenMapper(int screenWidth, int screenHeight)
    : screenWidth_(screenWidth), screenHeight_(screenHeight)
{}

void ScreenMapper::setScreenSize(int width, int height) {
    screenWidth_ = width;
    screenHeight_ = height;
}

/**
 * @brief Maps clip-space vertices to screen pixel coordinates.
 * 
 * For each input vertex in clip space, performs perspective division to get NDC (x/w, y/w),
 * converts from NDC [-1,1] to screen pixel coordinates, flips the y-axis to match screen orientation,
 * and clamps the results to the screen boundaries.
 * 
 * @param clipSpaceVertices Input vertices in clip-space as a vector of vec4.
 * @return std::vector<MiniGLM::ivec2> Vector of pixel coordinates on the screen.
 */
std::vector<MiniGLM::ivec2> ScreenMapper::mapToScreen(const std::vector<MiniGLM::vec4>& clipSpaceVertices) const
{
    std::vector<MiniGLM::ivec2> screenVerts;
    screenVerts.reserve(clipSpaceVertices.size());

    constexpr float epsilon = 1e-4f;
    for (const auto& v : clipSpaceVertices) {
        if (v.w < epsilon || !std::isfinite(v.w)) {
            continue;
        }

        float ndc_x = v.x / v.w;
        float ndc_y = v.y / v.w;

        if (std::abs(ndc_x) > 1.f || std::abs(ndc_y) > 1.f) {
            continue;
        }

        int px = static_cast<int>((ndc_x * 0.5f + 0.5f) * screenWidth_);
        int py = static_cast<int>((1.0f - (ndc_y * 0.5f + 0.5f)) * screenHeight_);

        px = std::clamp(px, 0, screenWidth_ - 1);
        py = std::clamp(py, 0, screenHeight_ - 1);

        screenVerts.emplace_back(MiniGLM::ivec2(px, py));
    }

    return screenVerts;
}

