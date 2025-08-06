#include "Mesh.hpp"
#include <vector>

Mesh::Mesh(const std::vector<glm::vec3>& vertices, const std::vector<std::pair<int, int>>& edges)
    : VAO_(0), VBO_(0), EBO_(0), edgeCount_(0) {
    setupMesh(vertices, edges);
}

Mesh::~Mesh() {
    if (EBO_) glDeleteBuffers(1, &EBO_);
    if (VBO_) glDeleteBuffers(1, &VBO_);
    if (VAO_) glDeleteVertexArrays(1, &VAO_);
}

Mesh::Mesh(Mesh&& other) noexcept
    : VAO_(other.VAO_), VBO_(other.VBO_), EBO_(other.EBO_), edgeCount_(other.edgeCount_) {
    other.VAO_ = other.VBO_ = other.EBO_ = 0;
    other.edgeCount_ = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        if (EBO_) glDeleteBuffers(1, &EBO_);
        if (VBO_) glDeleteBuffers(1, &VBO_);
        if (VAO_) glDeleteVertexArrays(1, &VAO_);

        VAO_ = other.VAO_; VBO_ = other.VBO_; EBO_ = other.EBO_; edgeCount_ = other.edgeCount_;
        other.VAO_ = other.VBO_ = other.EBO_ = 0;
        other.edgeCount_ = 0;
    }
    return *this;
}

void Mesh::setupMesh(const std::vector<glm::vec3>& vertices, const std::vector<std::pair<int, int>>& edges) {
    std::vector<GLuint> edgeIndices;
    edgeIndices.reserve(edges.size() * 2);
    for (const auto& e : edges) {
        edgeIndices.push_back(static_cast<GLuint>(e.first));
        edgeIndices.push_back(static_cast<GLuint>(e.second));
    }
    edgeCount_ = static_cast<GLsizei>(edgeIndices.size());

    glGenVertexArrays(1, &VAO_);
    glGenBuffers(1, &VBO_);
    glGenBuffers(1, &EBO_);

    glBindVertexArray(VAO_);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), reinterpret_cast<void*>(0));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, edgeIndices.size() * sizeof(GLuint), edgeIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Mesh::bind() const {
    glBindVertexArray(VAO_);
}

void Mesh::draw() const {
    glBindVertexArray(VAO_);
    glDrawElements(GL_LINES, edgeCount_, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
}

void Mesh::unbind() {
    glBindVertexArray(0);
}
