#pragma once

#include "MiniGLM.hpp"
#include <vector>

struct Color {
  unsigned char r, g, b, a;
  Color(unsigned char r_ = 0, unsigned char g_ = 0, unsigned char b_ = 0,
        unsigned char a_ = 255)
      : r(r_), g(g_), b(b_), a(a_) {}
};

class Rasterizer {
public:
  Rasterizer(int width, int height);

  void clear(const Color &color);

  void drawLine(const MiniGLM::ivec2 &p0, const MiniGLM::ivec2 &p1,
                const Color &color);

  void plotAA(int x, int y, const Color &color, float intensity);

  const std::vector<Color> &getBuffer() const { return buffer_; }

  int width() const { return width_; }
  int height() const { return height_; }

private:
  int width_, height_;
  std::vector<Color> buffer_;

  void setPixel(int x, int y, const Color &color);
};
