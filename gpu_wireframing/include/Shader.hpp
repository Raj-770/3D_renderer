#pragma once

#include "MiniGLM.hpp"
#include <glad/glad.h>
#include <string>

class Shader {
public:
  Shader(const std::string &vertexPath, const std::string &fragmentPath);

  ~Shader();

  Shader(const Shader &) = delete;
  Shader &operator=(const Shader &) = delete;

  Shader(Shader &&other) noexcept;
  Shader &operator=(Shader &&other) noexcept;

  void use() const;

  GLuint getID() const { return ID_; }

  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
  void setVec3(const std::string &name, const MiniGLM::vec3 &value) const;
  void setMat4(const std::string &name, const MiniGLM::mat4 &mat) const;

private:
  GLuint ID_;

  std::string loadShaderSource(const std::string &filepath) const;

  void checkCompileErrors(GLuint shader, const std::string &type) const;
};
