#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <iostream>
#include <string>
#include "ObjParser.hpp"
#include "MiniGLM.hpp"
#include "VertexProcessor.hpp"
#include "ScreenMapper.hpp"
#include "Rasterizer.hpp"


#include "stb_image_write.h"

constexpr int WINDOW_WIDTH = 1200;
constexpr int WINDOW_HEIGHT = 800;

int main(int argc, char **argv) {
    if (argc != 7) {
        std::cerr << "Usage: render-to-file input.obj cam_x cam_y cam_z [perspective|orthographic] output.png\n";
        return 1;
    }

    const char* objFile = argv[1];
    float camX = std::stof(argv[2]);
    float camY = std::stof(argv[3]);
    float camZ = std::stof(argv[4]);
    std::string projType = argv[5];
    const char* outFile = argv[6];

    // Parse OBJ
    ObjParser parser;
    if (!parser.load(objFile)) {
        std::cerr << "Failed to load OBJ file.\n";
        return 1;
    }
    std::cout << "Loaded " << parser.vertices.size() << " vertices and "
              << parser.faces.size() << " faces with "
              << parser.edges.size() << " edges.\n";

    MiniGLM::vec3 center = MiniGLM::vec3(0, 0, 0); // Place object at origin, as required
    MiniGLM::vec3 eye(camX, camY, camZ);
    MiniGLM::mat4 model = MiniGLM::mat4::identity();
    MiniGLM::mat4 view = MiniGLM::lookAt(eye, center, MiniGLM::vec3(0, 1, 0));

    MiniGLM::mat4 proj;
    if (projType == "perspective") {
        proj = MiniGLM::perspective(MiniGLM::radians(60.0f),
                                    float(WINDOW_WIDTH) / WINDOW_HEIGHT,
                                    0.01f, 100.0f);
    } else if (projType == "orthographic") {
        proj = MiniGLM::ortho(-6.0f, 6.0f, -4.0f, 4.0f, 0.01f, 100.0f);
    } else {
        std::cerr << "Unknown projection type (should be 'perspective' or 'orthographic').\n";
        return 1;
    }

    VertexProcessor processor(model, view, proj);
    ScreenMapper mapper(WINDOW_WIDTH, WINDOW_HEIGHT);
    Rasterizer raster(WINDOW_WIDTH, WINDOW_HEIGHT);

    // Transform and render
    raster.clear(Color(24, 24, 28)); // Background
    auto clip_space = processor.transformVertices(parser.vertices);
    auto screen_pts = mapper.mapToScreen(clip_space);

    Color white(255,255,255);
    for(const auto& edge : parser.edges) {
        raster.drawLine(screen_pts[edge.first], screen_pts[edge.second], white);
    }

    // Convert buffer to target format (PNG)
    const auto& buf = raster.getBuffer();
    std::vector<uint8_t> rgb(WINDOW_WIDTH * WINDOW_HEIGHT * 3);
    for(size_t i = 0; i < buf.size(); ++i) {
        rgb[i*3 + 0] = buf[i].r;
        rgb[i*3 + 1] = buf[i].g;
        rgb[i*3 + 2] = buf[i].b;
    }
    // Using stb_image_write (or swap for your image library)
    if (!stbi_write_png(outFile, WINDOW_WIDTH, WINDOW_HEIGHT, 3, rgb.data(), WINDOW_WIDTH * 3)) {
        std::cerr << "Could not write output PNG file.\n";
        return 1;
    }
    std::cout << "Rendered frame saved to " << outFile << "\n";
    return 0;
}
