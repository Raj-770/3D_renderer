#include "Rasterizer.hpp"
#include <fstream>
#include <algorithm>

Rasterizer::Rasterizer(int width, int height)
    : width_(width), height_(height), buffer_(width * height)
{}

void Rasterizer::clear(const Color& color) {
    std::fill(buffer_.begin(), buffer_.end(), color);
}

void Rasterizer::setPixel(int x, int y, const Color& color) {
    if (x >= 0 && x < width_ && y >= 0 && y < height_)
        buffer_[y * width_ + x] = color;
}

// Classic Bresenham's line algorithm (integer-based)
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

// Save as simple PPM image (for testing)
void Rasterizer::saveAsPPM(const std::string& filename) const {
    std::ofstream ofs(filename, std::ios::out | std::ios::binary);
    ofs << "P6\n" << width_ << " " << height_ << "\n255\n";
    for (const Color& c : buffer_) {
        ofs << (char)c.r << (char)c.g << (char)c.b;
    }
    ofs.close();
}
