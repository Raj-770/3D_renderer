#include "VertexProcessor.hpp"
#include <thread>

/**
 * @brief Construct a VertexProcessor with given model, view, and projection
 * matrix.
 *
 * @param model The model (object-to-world) transformation matrix
 * @param view The view (world-to-camera) transformation matrix
 * @param projection The projection (camera-to-clip-space) matrix
 */
VertexProcessor::VertexProcessor(const MiniGLM::mat4 &model,
                                 const MiniGLM::mat4 &view,
                                 const MiniGLM::mat4 &projection)
    : model_(model), view_(view), projection_(projection) {}

void VertexProcessor::setModelMatrix(const MiniGLM::mat4 &model) {
  model_ = model;
}

void VertexProcessor::setViewMatrix(const MiniGLM::mat4 &view) { view_ = view; }

void VertexProcessor::setProjectionMatrix(const MiniGLM::mat4 &projection) {
  projection_ = projection;
}

/**
 * @brief Applies the Model-View-Projection (MVP) trasformation to a list of
 * vertices. It takes a collection of object-space vertices, converts each into
 * a homogeneous coordinate (vec4) with w=1.0, and multiplies it by the combined
 * MVP matrix to obtain the transformed vertices in clip space.
 *
 * @param vertices The input vertices in object space as a vector of Vec3
 * @return std::vector<MiniGLM::vec4>  A vector containing the transformed
 * vertices in clip space.
 */
std::vector<MiniGLM::vec4> VertexProcessor::transformVertices(
    const std::vector<MiniGLM::vec3> &vertices) const {
  std::vector<MiniGLM::vec4> transformed(vertices.size());
  MiniGLM::mat4 mvp = projection_ * view_ * model_;

  const size_t numThreads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads(numThreads);

  auto worker = [&](size_t start, size_t end) {
    for (size_t i = start; i < end; ++i) {
      transformed[i] = mvp * MiniGLM::vec4(vertices[i], 1.0f);
    }
  };

  size_t chunkSize = vertices.size() / numThreads;
  for (size_t t = 0; t < numThreads; ++t) {
    size_t start = t * chunkSize;
    size_t end = (t + 1 == numThreads) ? vertices.size() : start + chunkSize;
    threads[t] = std::thread(worker, start, end);
  }
  for (auto &th : threads)
    th.join();

  return transformed;
}