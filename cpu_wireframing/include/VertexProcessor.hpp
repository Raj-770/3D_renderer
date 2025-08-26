#pragma once

#include "MiniGLM.hpp"
#include <vector>

class VertexProcessor {
public:
  VertexProcessor(const MiniGLM::mat4 &model, const MiniGLM::mat4 &view,
                  const MiniGLM::mat4 &projection);

  void setModelMatrix(const MiniGLM::mat4 &model);
  void setViewMatrix(const MiniGLM::mat4 &view);
  void setProjectionMatrix(const MiniGLM::mat4 &projection);

  std::vector<MiniGLM::vec4>
  transformVertices(const std::vector<MiniGLM::vec3> &vertices) const;

private:
  MiniGLM::mat4 model_;
  MiniGLM::mat4 view_;
  MiniGLM::mat4 projection_;
};
