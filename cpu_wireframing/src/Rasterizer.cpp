#include "Rasterizer.hpp"
#include <fstream>
#include <algorithm>

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

/**
 * @brief Draws a line between two points using Bresenham's algorithm.
 *
 * Implements Bresenham's line drawing algorithm, which efficiently determines
 * the raster pixels closest to a straight line between endpoints. The algorithm
 * swaps axes for steep lines, increments pixels along the major axis,
 * and sets pixel color using setPixel, ensuring continuous appearance
 * even for lines with arbitrary slope.
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
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    int dx = x1 - x0;
    int dy = std::abs(y1 - y0);
    int err = dx / 2;
    int ystep = (y0 < y1) ? 1 : -1;
    int y = y0;

    for (int x = x0; x <= x1; ++x) {
        if (steep)
            setPixel(y, x, color);
        else
            setPixel(x, y, color);
        err -= dy;
        if (err < 0) {
            y += ystep;
            err += dx;
        }
    }
}

