#include "WireframeApp.hpp"
#include <QMouseEvent>
#include <QPainter>
#include <QWheelEvent>
#include <QtMath>
#include <cstring>


WireframeApp::WireframeApp(const std::vector<MiniGLM::vec3> &vertices,
                           const std::vector<std::pair<int, int>> &edges,
                           int width, int height, QWidget *parent)
    : QWidget(parent), m_width(width), m_height(height), m_frameBuffer(nullptr),
      cam_dist_(30.0f), m_image(nullptr),
      processor(MiniGLM::mat4::identity(), MiniGLM::mat4::identity(),
                MiniGLM::mat4::identity()),
      raster(m_width, m_height), nearClipper(Clipper(0.01f)),
      screenClipper(Clipper(0, 0, m_width - 1, m_height - 1)),
      vertices(vertices), edges(edges) {
  allocateBuffer();
  updateImage();
  center = computeCenter(vertices);
  eye = center + MiniGLM::vec3(0, 0, cam_dist_);
  model = MiniGLM::mat4::identity();
  view = MiniGLM::lookAt(eye, center, MiniGLM::vec3(0, 1, 0));
  proj = MiniGLM::perspective(MiniGLM::radians(60.0f),
                              float(m_width) / m_height, 0.01f, 100.0f);

  MiniGLM::vec3 dir = normalize(eye - center);
pitch_ = std::asin(dir.y) * 180.0f / M_PI;
yaw_ = std::atan2(dir.x, dir.z) * 180.0f / M_PI;
cam_dist_ = length(eye - center);

  updateCameraQt();


  processor.setModelMatrix(model);
  processor.setViewMatrix(view);
  processor.setProjectionMatrix(proj);
  setMouseTracking(true);

  size_t numThreads = std::thread::hardware_concurrency();
  if (numThreads == 0)
    numThreads = 4;

  for (size_t t = 0; t < numThreads; ++t) {
    threadPool.emplace_back([this]() {
      while (true) {
        std::pair<size_t, size_t> task;
        {
          std::unique_lock<std::mutex> lock(queueMutex);
          queueCV.wait(
              lock, [this] { return !workQueue.empty() || quitFlag.load(); });

          if (quitFlag.load())
            break;

          task = workQueue.front();
          workQueue.pop();
        }
        this->drawEdgesInRange(workerClipSpace, task.first, task.second,
                               workerColor, workerNearEpsilon, workerNdcLimit);
        --tasksPending;
      }
    });
  }

  renderModel();
}

WireframeApp::~WireframeApp() { freeBuffer(); }

void WireframeApp::renderModel() {
  auto clip_space = processor.transformVertices(vertices);
  raster.clear(Color(0, 0, 0, 255));
  drawEdgesMultithreaded(clip_space);
  const auto &rasterBuffer = raster.getBuffer();
  for (int y = 0; y < m_height; ++y) {
    for (int x = 0; x < m_width; ++x) {
      int idx = y * m_width + x;
      const Color &color = rasterBuffer[idx];
      int fbIdx = idx * 4;
      m_frameBuffer[fbIdx + 0] = color.r;
      m_frameBuffer[fbIdx + 1] = color.g;
      m_frameBuffer[fbIdx + 2] = color.b;
      m_frameBuffer[fbIdx + 3] = color.a;
    }
  }
  updateImage();
  update();
}

void WireframeApp::drawEdgesMultithreaded(
    const std::vector<MiniGLM::vec4> &clip_space) {
  size_t numThreads = threadPool.size();
  size_t edgesPerThread = edges.size() / numThreads;

  // Store arguments for workers
  workerClipSpace = clip_space;
  workerColor = Color(255, 255, 255);
  workerNearEpsilon = 1e-3f;
  workerNdcLimit = 100.0f;
  tasksPending = numThreads;

  {
    std::lock_guard<std::mutex> lock(queueMutex);
    for (size_t t = 0; t < numThreads; ++t) {
      size_t start = t * edgesPerThread;
      size_t end =
          (t == numThreads - 1) ? edges.size() : (start + edgesPerThread);
      workQueue.push({start, end});
    }
  }
  queueCV.notify_all();

  // Wait for all threads to finish their chunk
  while (tasksPending > 0) {
    std::this_thread::sleep_for(
        std::chrono::microseconds(50)); // Avoid harsh busy waiting
  }
}

void WireframeApp::drawEdgesInRange(
    const std::vector<MiniGLM::vec4> &clip_space, size_t start, size_t end,
    const Color &color, float near_epsilon, float ndc_limit) {
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
    p0.x = static_cast<int>((ndc_x0 * 0.5f + 0.5f) * m_width);
    p0.y = static_cast<int>((1.0f - (ndc_y0 * 0.5f + 0.5f)) * m_height);
    p1.x = static_cast<int>((ndc_x1 * 0.5f + 0.5f) * m_width);
    p1.y = static_cast<int>((1.0f - (ndc_y1 * 0.5f + 0.5f)) * m_height);

    int dx = p0.x - p1.x, dy = p0.y - p1.y;
    if ((dx * dx + dy * dy) < 4)
      continue;

    if (screenClipper.clipLine(p0, p1))
      raster.drawLine(p0, p1, color);
  }
}

void WireframeApp::closeEvent(QCloseEvent *event) {
  quitFlag.store(true);
  queueCV.notify_all();

  for (auto &thread : threadPool)
    if (thread.joinable())
      thread.join();
  threadPool.clear();

  event->accept();
}

void WireframeApp::paintEvent(QPaintEvent * /*event*/) {
  QPainter painter(this);
  if (m_image)
    painter.drawImage(0, 0, *m_image);
}

void WireframeApp::resizeEvent(QResizeEvent* event) {
    m_width = event->size().width();
    m_height = event->size().height();
    freeBuffer();
    allocateBuffer();
    raster = Rasterizer(m_width, m_height);
    updateImage();

    proj = MiniGLM::perspective(
        MiniGLM::radians(60.0f),
        float(m_width) / float(m_height),
        0.01f,
        100.0f
    );
    processor.setProjectionMatrix(proj);

    renderModel();

    QWidget::resizeEvent(event);
}

void WireframeApp::allocateBuffer() {
  m_frameBuffer = new uchar[m_width * m_height * 4];
  std::memset(m_frameBuffer, 0, m_width * m_height * 4);
}

void WireframeApp::freeBuffer() {
  if (m_image) {
    delete m_image;
    m_image = nullptr;
  }
  if (m_frameBuffer) {
    delete[] m_frameBuffer;
    m_frameBuffer = nullptr;
  }
}

void WireframeApp::updateImage() {
  if (m_image)
    delete m_image;
  m_image =
      new QImage(m_frameBuffer, m_width, m_height, QImage::Format_RGBA8888);
}

MiniGLM::vec3
WireframeApp::computeCenter(const std::vector<MiniGLM::vec3> &vertices) {
  MiniGLM::vec3 minPos(1e9), maxPos(-1e9);
  for (const auto &v : vertices) {
    minPos.x = std::min(minPos.x, v.x);
    minPos.y = std::min(minPos.y, v.y);
    minPos.z = std::min(minPos.z, v.z);
    maxPos.x = std::max(maxPos.x, v.x);
    maxPos.y = std::max(maxPos.y, v.y);
    maxPos.z = std::max(maxPos.z, v.z);
  }
  return (minPos + maxPos) * 0.5f;
}

void WireframeApp::mousePressEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    rotating_ = true;
    lastMousePos_ = event->pos();
  }
}

void WireframeApp::mouseReleaseEvent(QMouseEvent *event) {
  if (event->button() == Qt::LeftButton) {
    rotating_ = false;
  }
}

void WireframeApp::mouseMoveEvent(QMouseEvent *event) {
  if (rotating_) {
    QPoint delta = event->pos() - lastMousePos_;
    lastMousePos_ = event->pos();

    float sensitivity = 0.5f;
    yaw_ += delta.x() * sensitivity;
    pitch_ += delta.y() * sensitivity;
    pitch_ = std::clamp(pitch_, -89.0f, 89.0f);

    updateCameraQt();
    renderModel();
  }
}

void WireframeApp::wheelEvent(QWheelEvent *event) {
  float delta = event->angleDelta().y() / 120.0f; // one "tick": 120
  float zoomSensitivity = 0.1f;
  cam_dist_ *= std::pow(0.9f, delta);

  cam_dist_ = std::clamp(cam_dist_, 0.1f, 150.0f);

  updateCameraQt();
  renderModel();
}

void WireframeApp::updateCameraQt() {
  float radYaw = MiniGLM::radians(yaw_);
  float radPitch = MiniGLM::radians(pitch_);

  MiniGLM::vec3 camDir{std::cos(radPitch) * std::sin(radYaw),
                       std::sin(radPitch),
                       std::cos(radPitch) * std::cos(radYaw)};

  eye = center + camDir * cam_dist_;
  view = MiniGLM::lookAt(eye, center, MiniGLM::vec3(0, 1, 0));
  processor.setViewMatrix(view);
}
