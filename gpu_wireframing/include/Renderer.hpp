#pragma once

#include "Mesh.hpp"
#include "ModelMatrix.hpp"
#include "ProjectionMatrix.hpp"
#include "Shader.hpp"
#include "ViewMatrix.hpp"
#include <GLFW/glfw3.h>

class Renderer {
public:
  Renderer(GLFWwindow *window, Mesh &mesh, Shader &shader, ModelMatrix &model,
           ViewMatrix &view, ProjectionMatrix &projection);
  ~Renderer();

  // Render a frame (call each loop iteration)
  void renderFrame();

  // Callback or manual call to update viewport and projection
  void onWindowResize(int width, int height);

private:
  GLFWwindow *window_;
  Mesh &mesh_;
  Shader &shader_;
  ModelMatrix &model_;
  ViewMatrix &view_;
  ProjectionMatrix &projection_;

  int windowWidth_;
  int windowHeight_;

  void updateProjectionMatrix();
};
