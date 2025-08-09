#pragma once

#include <vector>
#include <utility>
#include <glad/glad.h>
#include "MiniGLM.hpp"

class Mesh {
public:
    Mesh(const std::vector<MiniGLM::vec3>& vertices, const std::vector<std::pair<int, int>>& edges);

    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    void bind() const;
    void draw() const;
    static void unbind();

private:
    GLuint VAO_;
    GLuint VBO_;
    GLuint EBO_;
    GLsizei edgeCount_;

    void setupMesh(const std::vector<MiniGLM::vec3>& vertices, const std::vector<std::pair<int, int>>& edges);
};