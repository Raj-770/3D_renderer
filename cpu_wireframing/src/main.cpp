#include <iostream>
#include <string>
#include <vector>
#include "ObjParser.hpp"
#include "MiniGLM.hpp"
#include "VertexProcessor.hpp"
#include "ScreenMapper.hpp"
#include "Rasterizer.hpp"
#include "InputHandler.hpp"
#include "MiniFB.h"

constexpr int WINDOW_WIDTH = 1200;
constexpr int WINDOW_HEIGHT = 800;

MiniGLM::vec3 compute_center(const std::vector<MiniGLM::vec3>& vertices) {
    MiniGLM::vec3 minPos(1e9), maxPos(-1e9);
    for(const auto& v : vertices) {
        minPos.x = std::min(minPos.x, v.x);
        minPos.y = std::min(minPos.y, v.y);
        minPos.z = std::min(minPos.z, v.z);
        maxPos.x = std::max(maxPos.x, v.x);
        maxPos.y = std::max(maxPos.y, v.y);
        maxPos.z = std::max(maxPos.z, v.z);
    }
    return (minPos + maxPos) * 0.5f;
}

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: ./framer <.obj file>\n";
        return 1;
    }

    ObjParser parser;
    if (!parser.load(argv[1])) {
        std::cerr << "Failed to load OBJ file.\n";
        return 1;
    }
    std::cout << "Loaded " << parser.vertices.size() << " vertices and "
              << parser.faces.size() << " faces with "
              << parser.edges.size() << " edges.\n";

    MiniGLM::mat4 model = MiniGLM::mat4::identity();
    MiniGLM::vec3 center = compute_center(parser.vertices);
    float cam_dist = 50.0f;
    MiniGLM::vec3 eye = center + MiniGLM::vec3(0, 0, cam_dist);
    MiniGLM::mat4 view = MiniGLM::lookAt(eye, center, MiniGLM::vec3(0,1,0));
    MiniGLM::mat4 proj = MiniGLM::perspective(MiniGLM::radians(60.0f), float(WINDOW_WIDTH)/WINDOW_HEIGHT, 0.01f, 100.0f);

    VertexProcessor processor(model, view, proj);
    ScreenMapper mapper(WINDOW_WIDTH, WINDOW_HEIGHT);
    Rasterizer raster(WINDOW_WIDTH, WINDOW_HEIGHT);

    std::vector<uint32_t> pixelBuffer(WINDOW_WIDTH * WINDOW_HEIGHT, 0);

    struct mfb_window* window = mfb_open("CPU Wireframe", WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!window) {
        std::cerr << "Could not create MiniFB window.\n";
        return 1;
    }

    InputHandler input;

    // Main loop
    while (mfb_wait_sync(window)) {

        bool changed = input.update(window);

        // Mouse scroll zoom:
        if (changed) {
            float scrollDelta = input.getScrollY();
            if (scrollDelta != 0.0f) {
                cam_dist *= std::pow(0.9f, scrollDelta);
                eye = center + MiniGLM::vec3(0, 0, cam_dist);
                view = MiniGLM::lookAt(eye, center, MiniGLM::vec3(0,1,0));
                processor.setViewMatrix(view);
            }
        }
        raster.clear(Color(24, 24, 28)); // dark bg

        // Transform pipeline: 3D → clip → NDC → screen
        auto clip_space = processor.transformVertices(parser.vertices);
        auto screen_pts = mapper.mapToScreen(clip_space);

        // Draw all edges
        Color white(255,255,255);
        for(const auto& edge : parser.edges) {
            raster.drawLine(screen_pts[edge.first], screen_pts[edge.second], white);
        }

        // Pack buffer for MiniFB: Rasterizer uses Color (r,g,b,a), MiniFB wants 0xRRGGBB
        const auto& buf = raster.getBuffer();
        for(size_t i=0; i<buf.size(); ++i) {
            const Color& c = buf[i];
            // MiniFB expects 0x00RRGGBB (lowest byte B)
            pixelBuffer[i] = (0xFF << 24) | (c.r << 16) | (c.g << 8) | c.b;
        }

        mfb_update(window, pixelBuffer.data());
        // if (mfb_get_key(window, KB_KEY_ESCAPE)) break;
    }

    mfb_close(window);
    return 0;
}
