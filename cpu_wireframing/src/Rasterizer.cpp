#include "Rasterizer.hpp"
#include <fstream>
#include <algorithm>
#include <cmath>


/**
 * @brief Constructs a Rasterizer object for a given image width and height.
 *
 * Initializes an internal pixel buffer to the correct size, preparing for raster operations
 * like clearing the screen, setting individual pixels, and drawing lines.
 *
 * @param width Width of the output image in pixels.
 * @param height Height of the output image in pixels.
 */
Rasterizer::Rasterizer(int width, int height)
    : width_(width), height_(height), buffer_(width * height)
{}

/**
 * @brief Clears the pixel buffer to a specified color.
 *
 * Sets every pixel in the buffer to the given color, effectively resetting the screen
 * before new drawing operations begin.
 *
 * @param color The color to fill every pixel of the buffer.
 */
void Rasterizer::clear(const Color& color) {
    std::fill(buffer_.begin(), buffer_.end(), color);
}

/**
 * @brief Sets the color of a single pixel at (x, y).
 *
 * Changes the color of the pixel at the specified coordinates to the given value
 * if the coordinates are within the screen boundaries.
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color The new color for the pixel.
 */
void Rasterizer::setPixel(int x, int y, const Color& color) {
    if (x >= 0 && x < width_ && y >= 0 && y < height_)
        buffer_[y * width_ + x] = color;
}

inline float frac(float x) { return x - std::floor(x); }

void Rasterizer::plotAA(int x, int y, const Color& color, float intensity) {
    if (x >= 0 && x < width_ && y >= 0 && y < height_ && intensity > 0.0f) {
        Color& dest = buffer_[y * width_ + x];
        dest.r = static_cast<uint8_t>(color.r * intensity + dest.r * (1.0f - intensity));
        dest.g = static_cast<uint8_t>(color.g * intensity + dest.g * (1.0f - intensity));
        dest.b = static_cast<uint8_t>(color.b * intensity + dest.b * (1.0f - intensity));
    }
}

/**
 * @brief Draws an anti-aliased line between two points using Xiaolin Wu's algorithm.
 *
 * This method renders a smooth line by blending pixel colors according to their proximity
 * to the mathematically precise line path, greatly reducing staircase (aliasing) artifacts.
 * It adapts automatically for steep lines by swapping axes, calculates fractional pixel
 * coverage, and uses the plotAA helper for intensity-based color blending.
 *
 * @param p0 The starting point of the line (as integer pixel coordinates).
 * @param p1 The ending point of the line (as integer pixel coordinates).
 * @param color The color to use when drawing the line.
 */
void Rasterizer::drawLine(const MiniGLM::ivec2& p0, const MiniGLM::ivec2& p1, const Color& color) {
    int x0 = p0.x, y0 = p0.y;
    int x1 = p1.x, y1 = p1.y;
    bool steep = std::abs(y1 - y0) > std::abs(x1 - x0);

    if (steep) {
        std::swap(x0, y0); std::swap(x1, y1);
    }
    if (x0 > x1) {
        std::swap(x0, x1); std::swap(y0, y1);
    }

    float dx = float(x1 - x0);
    float dy = float(y1 - y0);
    float gradient = dx == 0.0f ? 1.0f : dy / dx;

    int xEnd = x0;
    float yEnd = y0 + gradient * (xEnd - x0);
    int xPixel1 = xEnd;
    int yPixel1 = int(yEnd);

    if (steep) {
        plotAA(yPixel1, xPixel1, color, 1 - frac(yEnd));
        plotAA(yPixel1 + 1, xPixel1, color, frac(yEnd));
    } else {
        plotAA(xPixel1, yPixel1, color, 1 - frac(yEnd));
        plotAA(xPixel1, yPixel1 + 1, color, frac(yEnd));
    }
    float y = yEnd + gradient;

    for (int x = xPixel1 + 1; x < x1; ++x) {
        int yPix = int(y);
        if (steep) {
            plotAA(yPix, x, color, 1 - frac(y));
            plotAA(yPix + 1, x, color, frac(y));
        } else {
            plotAA(x, yPix, color, 1 - frac(y));
            plotAA(x, yPix + 1, color, frac(y));
        }
        y += gradient;
    }

    int xPixel2 = x1;
    float yPixel2 = y1;
    if (steep) {
        plotAA(int(yPixel2), xPixel2, color, 1 - frac(yPixel2));
        plotAA(int(yPixel2) + 1, xPixel2, color, frac(yPixel2));
    } else {
        plotAA(xPixel2, int(yPixel2), color, 1 - frac(yPixel2));
        plotAA(xPixel2, int(yPixel2) + 1, color, frac(yPixel2));
    }
}
