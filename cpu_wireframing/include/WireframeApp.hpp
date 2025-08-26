#pragma once

#include <Clipper.hpp>
#include <MiniGLM.hpp>
#include <QImage>
#include <QPoint>
#include <QResizeEvent>
#include <QWidget>
#include <Rasterizer.hpp>
#include <VertexProcessor.hpp>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

class WireframeApp : public QWidget {
  Q_OBJECT
public:
  explicit WireframeApp(const std::vector<MiniGLM::vec3> &vertices,
                        const std::vector<std::pair<int, int>> &edges,
                        int width, int height, QWidget *parent = nullptr);
  ~WireframeApp();

  void updateFrameBuffer(const uchar *data, int dataSize);

protected:
  void paintEvent(QPaintEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;
  void closeEvent(QCloseEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;
  void mouseMoveEvent(QMouseEvent *event) override;
  void wheelEvent(QWheelEvent *event) override;
  void mouseReleaseEvent(QMouseEvent *event) override;

private:
  int m_width;
  int m_height;
  uchar *m_frameBuffer;
  QImage *m_image;
  MiniGLM::vec3 center;
  float cam_dist;
  MiniGLM::vec3 eye;
  MiniGLM::mat4 model, view, proj;

  bool rotating_ = false;
  QPoint lastMousePos_;
  float yaw_ = 0.0f, pitch_ = 0.0f;
  float cam_dist_ = 2.5f;

  VertexProcessor processor;
  Rasterizer raster;
  Clipper nearClipper;
  Clipper screenClipper;

  std::vector<MiniGLM::vec3> vertices;
  std::vector<std::pair<int, int>> edges;

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

  void renderModel();

  void drawEdgesMultithreaded(const std::vector<MiniGLM::vec4> &clip_space);
  void drawEdgesInRange(const std::vector<MiniGLM::vec4> &clip_space,
                        size_t start, size_t end, const Color &color,
                        float near_epsilon, float ndc_limit);

  void allocateBuffer();
  void freeBuffer();
  void updateImage();

  MiniGLM::vec3 computeCenter(const std::vector<MiniGLM::vec3> &vertices);

  void updateCameraQt();
    
  
};
