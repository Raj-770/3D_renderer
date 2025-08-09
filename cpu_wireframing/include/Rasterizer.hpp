#pragma once

#include <vector>
#include "MiniGLM.hpp" // Contains ivec2

struct Color {
    unsigned char r, g, b, a;
    Color(unsigned char r_=0, unsigned char g_=0, unsigned char b_=0, unsigned char a_=255)
        : r(r_), g(g_), b(b_), a(a_) {}
};

class Rasterizer {
public:
    Rasterizer(int width, int height);

    // Clear the buffer with a color
    void clear(const Color& color);

    // Draw a single line (Bresenham) from p0 to p1
    void drawLine(const MiniGLM::ivec2& p0, const MiniGLM::ivec2& p1, const Color& color);

    // Get the raw pixel buffer (RGBA)
    const std::vector<Color>& getBuffer() const { return buffer_; }

    // Write buffer to image file (optional, for demonstration)
    void saveAsPPM(const std::string& filename) const;

    int width() const { return width_; }
    int height() const { return height_; }

private:
    int width_, height_;
    std::vector<Color> buffer_;

    // Helper: set a pixel if in bounds
    void setPixel(int x, int y, const Color& color);
};
