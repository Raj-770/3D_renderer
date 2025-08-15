#include "WireframeApp.hpp"
#include <algorithm>
#include <cmath>
#include <thread>
#include <vector>

MiniGLM::vec3 compute_center(const std::vector<MiniGLM::vec3> &vertices)
{
    MiniGLM::vec3 minPos(1e9), maxPos(-1e9);
    for (const auto &v : vertices)
    {
        minPos.x = std::min(minPos.x, v.x);
        minPos.y = std::min(minPos.y, v.y);
        minPos.z = std::min(minPos.z, v.z);
        maxPos.x = std::max(maxPos.x, v.x);
        maxPos.y = std::max(maxPos.y, v.y);
        maxPos.z = std::max(maxPos.z, v.z);
    }
    return (minPos + maxPos) * 0.5f;
}

WireframeApp::WireframeApp(
    const std::vector<MiniGLM::vec3> &vertices,
    const std::vector<std::pair<int, int>> &edges,
    int windowWidth,
    int windowHeight)
    : windowWidth(windowWidth),
      windowHeight(windowHeight),
      cam_dist(50.0f),
      processor(MiniGLM::mat4::identity(), MiniGLM::mat4::identity(), MiniGLM::mat4::identity()),
      mapper(windowWidth, windowHeight),
      raster(windowWidth, windowHeight),
      pixelBuffer(windowWidth * windowHeight, 0),
      vertices(vertices),
      edges(edges),
      nearClipper(Clipper(0.01f)),
      screenClipper(Clipper(0, 0, windowWidth - 1, windowHeight - 1))
{
    center = compute_center(vertices);
    eye = center + MiniGLM::vec3(0, 0, cam_dist);
    model = MiniGLM::mat4::identity();
    view = MiniGLM::lookAt(eye, center, MiniGLM::vec3(0, 1, 0));
    proj = MiniGLM::perspective(MiniGLM::radians(60.0f), float(windowWidth) / windowHeight, 0.01f, 100.0f);

    processor.setModelMatrix(model);
    processor.setViewMatrix(view);
    processor.setProjectionMatrix(proj);
}

void WireframeApp::run()
{
    struct mfb_window *window = mfb_open("CPU Wireframe", windowWidth, windowHeight);
    if (!window)
    {
        fprintf(stderr, "Could not create MiniFB window.\n");
        return;
    }
    static float yaw = 0.0f, pitch = 0.0f;
    while (mfb_wait_sync(window))
    {
        input.updateRotation(window);
        bool changedScroll = input.updateZoom(window);
        if (changedScroll)
        {
            float scrollDelta = input.getScrollY();
            if (scrollDelta != 0.0f)
            {
                cam_dist *= std::pow(0.9f, scrollDelta);
                eye = center + MiniGLM::vec3(0, 0, cam_dist);
                view = MiniGLM::lookAt(eye, center, MiniGLM::vec3(0, 1, 0));
                processor.setViewMatrix(view);
            }
        }

        if (input.isRotating())
        {
            yaw += input.getDeltaX() * 0.3f;
            pitch += input.getDeltaY() * 0.3f;
            pitch = std::clamp(pitch, -89.0f, 89.0f);
        }

        float radYaw = MiniGLM::radians(yaw);
        float radPitch = MiniGLM::radians(pitch);
        MiniGLM::vec3 camDir = {
            std::cos(radPitch) * std::sin(radYaw),
            std::sin(radPitch),
            std::cos(radPitch) * std::cos(radYaw)};
        eye = center + camDir * cam_dist;
        view = MiniGLM::lookAt(eye, center, MiniGLM::vec3(0, 1, 0));
        processor.setViewMatrix(view);

        raster.clear(Color(24, 24, 28));

        auto clip_space = processor.transformVertices(vertices);
        auto screen_pts = mapper.mapToScreen(clip_space);

        Color white(255, 255, 255);
        size_t numThreads = std::thread::hardware_concurrency();
        if (numThreads == 0)
            numThreads = 4; // Fallback

        std::vector<std::thread> threads;
        size_t edgesPerThread = edges.size() / numThreads;

        constexpr float near_epsilon = 1e-3f;
        constexpr float ndc_limit = 2.0f;

        for (size_t t = 0; t < numThreads; ++t)
        {
            size_t start = t * edgesPerThread;
            size_t end = (t == numThreads - 1) ? edges.size() : (start + edgesPerThread);

            threads.emplace_back([&, start, end]()
                                 {
                for (size_t i = start; i < end; ++i) {
                    MiniGLM::vec4 clipV0 = clip_space[edges[i].first];
                    MiniGLM::vec4 clipV1 = clip_space[edges[i].second];

                    if (!nearClipper.clipLineNearPlane(clipV0, clipV1))
                        continue;

                    if (clipV0.w < near_epsilon || clipV1.w < near_epsilon)
                        continue;

                    float ndc_x0 = clipV0.x / clipV0.w;
                    float ndc_y0 = clipV0.y / clipV0.w;
                    float ndc_x1 = clipV1.x / clipV1.w;
                    float ndc_y1 = clipV1.y / clipV1.w;

                    auto excess = [](float v) { return std::max(0.0f, fabs(v) - 1.0f); };

                    float ndc_x0_excess = excess(ndc_x0);
                    float ndc_y0_excess = excess(ndc_y0);
                    float ndc_x1_excess = excess(ndc_x1);
                    float ndc_y1_excess = excess(ndc_y1);

                    float threshold = ndc_limit - 1.0f;

                    bool p0_far = std::max(ndc_x0_excess, ndc_y0_excess) > threshold;
                    bool p1_far = std::max(ndc_x1_excess, ndc_y1_excess) > threshold;

                    if (p0_far || p1_far)
                        continue;

        
                    MiniGLM::ivec2 p0, p1;
                    p0.x = static_cast<int>((ndc_x0 * 0.5f + 0.5f) * windowWidth);
                    p0.y = static_cast<int>((1.0f - (ndc_y0 * 0.5f + 0.5f)) * windowHeight);
                    p1.x = static_cast<int>((ndc_x1 * 0.5f + 0.5f) * windowWidth);
                    p1.y = static_cast<int>((1.0f - (ndc_y1 * 0.5f + 0.5f)) * windowHeight);

                    int dx = p0.x - p1.x, dy = p0.y - p1.y;
                    if ((dx*dx + dy*dy) < 4)
                        continue;

                    if (screenClipper.clipLine(p0, p1))
                        raster.drawLine(p0, p1, white);
            
                } });
        }

        for (auto &th : threads)
            th.join();

        const auto &buf = raster.getBuffer();
        for (size_t i = 0; i < buf.size(); ++i)
        {
            const Color &c = buf[i];
            pixelBuffer[i] = (0xFF << 24) | (c.r << 16) | (c.g << 8) | c.b;
        }

        mfb_update(window, pixelBuffer.data());
    }

    mfb_close(window);
}
