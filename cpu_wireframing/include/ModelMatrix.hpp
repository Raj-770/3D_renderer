#include "MiniGLM.hpp"

class ModelMatrix {
public:
  ModelMatrix();

  void setTranslation(const MiniGLM::vec3 &translation);
  void setRotation(float angleDegrees, const MiniGLM::vec3 &axis);
  void setScale(const MiniGLM::vec3 &sclae);

  MiniGLM::mat4 getModelMatrix() const;

private:
  MiniGLM::vec3 translation_;
  float rotationAngle_;
  MiniGLM::vec3 rotationAxis_;
  MiniGLM::vec3 scale_;

  mutable MiniGLM::mat4 model_;
  mutable bool dirty_;

  void updateModelMatrix() const;
};