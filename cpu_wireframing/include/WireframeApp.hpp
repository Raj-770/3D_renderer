#pragma once

#include <vector>
#include <MiniGLM.hpp>
#include <VertexProcessor.hpp>
#include <Rasterizer.hpp>
#include <InputHandler.hpp>
#include <MiniFB.h>
#include "ObjParser.hpp"
#include "Clipper.hpp"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <atomic>
#include <thread>


class WireframeApp {
public:
    WireframeApp(const std::vector<MiniGLM::vec3>& vertices,
                 const std::vector<std::pair<int, int>>& edges,
                 int windowWidth,
                 int windowHeight);

    void run();

    ~WireframeApp();

private:
    int windowWidth, windowHeight;
    MiniGLM::vec3 center;
    float cam_dist;
    MiniGLM::vec3 eye;
    MiniGLM::mat4 model, view, proj;
    

    VertexProcessor processor;
    Rasterizer raster;
    InputHandler input;
    std::vector<uint32_t> pixelBuffer;

    std::vector<MiniGLM::vec3> vertices;
    std::vector<std::pair<int, int>> edges;

    Clipper nearClipper;
    Clipper screenClipper;

    std::vector<std::thread> threadPool;
    std::queue<std::pair<size_t, size_t>> workQueue;
    std::mutex queueMutex;
    std::condition_variable queueCV;
    std::atomic<bool> quitFlag{false};
    std::atomic<int> tasksPending{0};

    std::vector<MiniGLM::vec4> workerClipSpace;
    Color workerColor = Color(255, 255, 255);
    float workerNearEpsilon = 1e-3f;
    float workerNdcLimit = 100.0f;


    MiniGLM::vec3 computeCenter(const std::vector<MiniGLM::vec3> &vertices);
    struct mfb_window* initWindow();
    void handleInput(struct mfb_window* window, float& yawVel, float& pitchVel, float& zoomVel);
    void adjustZoom();
    void updateCamera(float yaw, float pitch, float cam_dist);
    void drawEdgesMultithreaded(const std::vector<MiniGLM::vec4>& clip_space);
    void drawEdgesInRange(const std::vector<MiniGLM::vec4>& clip_space, size_t start, size_t end, const Color& color, float near_epsilon, float ndc_limit);
    void updatePixelBuffer();

};
