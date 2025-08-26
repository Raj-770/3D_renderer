#include "Clipper.hpp"
#include <algorithm>

// Screen-space clipping constructor
Clipper::Clipper(int xmin, int ymin, int xmax, int ymax)
    : xmin_(xmin), ymin_(ymin), xmax_(xmax), ymax_(ymax), nearPlane_(0.0f) {}

// 3D near plane clipping constructor
Clipper::Clipper(float nearPlane)
    : xmin_(0), ymin_(0), xmax_(0), ymax_(0), nearPlane_(nearPlane) {}

int Clipper::computeOutCode(int x, int y) const {
  int code = INSIDE;
  if (x < xmin_)
    code |= LEFT;
  else if (x > xmax_)
    code |= RIGHT;
  if (y < ymin_)
    code |= BOTTOM;
  else if (y > ymax_)
    code |= TOP;
  return code;
}

MiniGLM::vec4 Clipper::lerp(const MiniGLM::vec4 &a, const MiniGLM::vec4 &b,
                            float t) const {
  return MiniGLM::vec4(a.x + t * (b.x - a.x), a.y + t * (b.y - a.y),
                       a.z + t * (b.z - a.z), a.w + t * (b.w - a.w));
}

bool Clipper::clipLineNearPlane(MiniGLM::vec4 &p0, MiniGLM::vec4 &p1) const {
  // In clip space, the near plane test is: w >= nearPlane
  // For standard perspective projection, nearPlane is typically the near
  // distance

  bool p0Inside = p0.w >= nearPlane_;
  bool p1Inside = p1.w >= nearPlane_;

  // Both points in front of near plane - accept
  if (p0Inside && p1Inside) {
    return true;
  }

  // Both points behind near plane - reject
  if (!p0Inside && !p1Inside) {
    return false;
  }

  // One point is behind near plane - clip the line
  if (!p0Inside) {
    // p0 is behind, p1 is in front
    float t = (nearPlane_ - p0.w) / (p1.w - p0.w);
    p0 = lerp(p0, p1, t);
  } else {
    // p1 is behind, p0 is in front
    float t = (nearPlane_ - p1.w) / (p0.w - p1.w);
    p1 = lerp(p1, p0, t);
  }

  return true;
}

// Existing screen clipping function (unchanged)
bool Clipper::clipLine(MiniGLM::ivec2 &p0, MiniGLM::ivec2 &p1) const {
  int x0 = p0.x, y0 = p0.y;
  int x1 = p1.x, y1 = p1.y;
  int outcode0 = computeOutCode(x0, y0);
  int outcode1 = computeOutCode(x1, y1);

  bool accept = false;

  while (true) {
    if (!(outcode0 | outcode1)) {
      accept = true;
      break;
    } else if (outcode0 & outcode1) {
      break;
    } else {
      int outcodeOut = outcode0 ? outcode0 : outcode1;
      int x, y;

      if (outcodeOut & TOP) {
        x = x0 + (x1 - x0) * (ymax_ - y0) / (y1 - y0);
        y = ymax_;
      } else if (outcodeOut & BOTTOM) {
        x = x0 + (x1 - x0) * (ymin_ - y0) / (y1 - y0);
        y = ymin_;
      } else if (outcodeOut & RIGHT) {
        y = y0 + (y1 - y0) * (xmax_ - x0) / (x1 - x0);
        x = xmax_;
      } else { // LEFT
        y = y0 + (y1 - y0) * (xmin_ - x0) / (x1 - x0);
        x = xmin_;
      }

      if (outcodeOut == outcode0) {
        x0 = x;
        y0 = y;
        outcode0 = computeOutCode(x0, y0);
      } else {
        x1 = x;
        y1 = y;
        outcode1 = computeOutCode(x1, y1);
      }
    }
  }

  if (accept) {
    p0.x = x0;
    p0.y = y0;
    p1.x = x1;
    p1.y = y1;
    return true;
  }
  return false;
}
