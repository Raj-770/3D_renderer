#pragma once

#include <vector>
#include <MiniGLM.hpp>
#include <VertexProcessor.hpp>
#include <ScreenMapper.hpp>
#include <Rasterizer.hpp>
#include <InputHandler.hpp>
#include <MiniFB.h>
#include "ObjParser.hpp"

class WireframeApp {
public:
    WireframeApp(const std::vector<MiniGLM::vec3>& vertices,
                 const std::vector<std::pair<int, int>>& edges,
                 int windowWidth,
                 int windowHeight);

    void run();

private:
    int windowWidth, windowHeight;
    MiniGLM::vec3 center;
    float cam_dist;
    MiniGLM::vec3 eye;
    MiniGLM::mat4 model, view, proj;

    VertexProcessor processor;
    ScreenMapper mapper;
    Rasterizer raster;
    InputHandler input;
    std::vector<uint32_t> pixelBuffer;

    std::vector<MiniGLM::vec3> vertices;
    std::vector<std::pair<int, int>> edges;

    void computeCenter();
};
