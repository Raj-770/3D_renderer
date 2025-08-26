#include "MiniGLM.hpp"
#include "ObjParser.hpp"
#include "Rasterizer.hpp"
#include "VertexProcessor.hpp"
#include <QImage>
#include <QString>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>
#include <vector>

constexpr int WINDOW_WIDTH = 1000;
constexpr int WINDOW_HEIGHT = 1000;

MiniGLM::ivec2 ndcToScreen(const MiniGLM::vec4 &ndc, int width, int height) {
  int x = static_cast<int>((ndc.x * 0.5f + 0.5f) * width);
  int y = static_cast<int>((1.0f - (ndc.y * 0.5f + 0.5f)) * height);
  return {x, y};
}

// Simple line clip against screen bounds
bool clipScreenLine(MiniGLM::ivec2 &p0, MiniGLM::ivec2 &p1, int width,
                    int height) {
  if ((p0.x < 0 && p1.x < 0) || (p0.x >= width && p1.x >= width))
    return false;
  if ((p0.y < 0 && p1.y < 0) || (p0.y >= height && p1.y >= height))
    return false;
  return true;
}

int main(int argc, char **argv) {
  if (argc != 7) {
    std::cerr << "Usage: render-to-file input.obj cam_x cam_y cam_z "
                 "[perspective|orthographic] output.png\n";
    return 1;
  }

  const char *objFile = argv[1];
  float camX = std::stof(argv[2]);
  float camY = std::stof(argv[3]);
  float camZ = std::stof(argv[4]);
  std::string projType = argv[5];
  const char *outFile = argv[6];

  // Parse OBJ
  ObjParser parser;
  if (!parser.load(objFile)) {
    std::cerr << "Failed to load OBJ file.\n";
    return 1;
  }
  std::cout << "Loaded " << parser.vertices.size() << " vertices and "
            << parser.faces.size() << " faces with " << parser.edges.size()
            << " edges.\n";

  MiniGLM::vec3 center(0, 0, 0); // Place object at origin, as required
  MiniGLM::vec3 eye(camX, camY, camZ);
  MiniGLM::mat4 model = MiniGLM::mat4::identity();
  MiniGLM::mat4 view = MiniGLM::lookAt(eye, center, MiniGLM::vec3(0, 1, 0));

  MiniGLM::mat4 proj;
  if (projType == "perspective") {
    proj = MiniGLM::perspective(MiniGLM::radians(100.0f),
                                float(WINDOW_WIDTH) / WINDOW_HEIGHT, 0.01f,
                                100.0f);
  } else if (projType == "orthographic") {
    proj = MiniGLM::ortho(-6.0f, 6.0f, -4.0f, 4.0f, 0.01f, 100.0f);
  } else {
    std::cerr << "Unknown projection type (should be 'perspective' or "
                 "'orthographic').\n";
    return 1;
  }

  VertexProcessor processor(model, view, proj);
  Rasterizer raster(WINDOW_WIDTH, WINDOW_HEIGHT);

  raster.clear(Color(24, 24, 28)); // Background

  // Transform vertices to clip space
  auto clip_space = processor.transformVertices(parser.vertices);

  // Draw edges with near/far clipping and NDC mapping
  Color white(255, 255, 255);
  constexpr float near_epsilon = 1e-3f;
  constexpr float ndc_limit = 100.0f;

  auto excess = [](float v) { return std::max(0.0f, std::fabs(v) - 1.0f); };
  float threshold = ndc_limit - 1.0f;

  for (const auto &edge : parser.edges) {
    MiniGLM::vec4 clipV0 = clip_space[edge.first];
    MiniGLM::vec4 clipV1 = clip_space[edge.second];

    // Near plane clipping
    if (clipV0.w < near_epsilon || clipV1.w < near_epsilon)
      continue;

    // NDC mapping
    float ndc_x0 = clipV0.x / clipV0.w;
    float ndc_y0 = clipV0.y / clipV0.w;
    float ndc_x1 = clipV1.x / clipV1.w;
    float ndc_y1 = clipV1.y / clipV1.w;

    // Filter extreme far points
    float ndc_x0_excess = excess(ndc_x0);
    float ndc_y0_excess = excess(ndc_y0);
    float ndc_x1_excess = excess(ndc_x1);
    float ndc_y1_excess = excess(ndc_y1);

    bool p0_far = std::max(ndc_x0_excess, ndc_y0_excess) > threshold;
    bool p1_far = std::max(ndc_x1_excess, ndc_y1_excess) > threshold;

    if (p0_far || p1_far)
      continue;

    // Map NDC to screen space
    MiniGLM::ivec2 p0 =
        ndcToScreen({ndc_x0, ndc_y0, 0, 0}, WINDOW_WIDTH, WINDOW_HEIGHT);
    MiniGLM::ivec2 p1 =
        ndcToScreen({ndc_x1, ndc_y1, 0, 0}, WINDOW_WIDTH, WINDOW_HEIGHT);

    int dx = p0.x - p1.x, dy = p0.y - p1.y;
    if ((dx * dx + dy * dy) < 4)
      continue;

    if (clipScreenLine(p0, p1, WINDOW_WIDTH, WINDOW_HEIGHT))
      raster.drawLine(p0, p1, white);
  }

  // PNG export
  const auto &buf = raster.getBuffer();
  std::vector<uint8_t> rgba(WINDOW_WIDTH * WINDOW_HEIGHT * 4);
  for (size_t i = 0; i < buf.size(); ++i) {
    rgba[i * 4 + 0] = buf[i].r;
    rgba[i * 4 + 1] = buf[i].g;
    rgba[i * 4 + 2] = buf[i].b;
    rgba[i * 4 + 3] = 255; // fully opaque
  }

  QImage image(rgba.data(), WINDOW_WIDTH, WINDOW_HEIGHT,
               QImage::Format_RGBA8888);
  // If you need RGB flip (top <-> bottom), add .mirrored()
  if (!image.save(QString::fromUtf8(outFile), "PNG")) {
    std::cerr << "Could not write output PNG file.\n";
    return 1;
  }
  std::cout << "Rendered frame saved to " << outFile << "\n";
  return 0;
}
