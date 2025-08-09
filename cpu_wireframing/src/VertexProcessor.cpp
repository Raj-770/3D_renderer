#include "VertexProcessor.hpp"

VertexProcessor::VertexProcessor(
    const MiniGLM::mat4& model,
    const MiniGLM::mat4& view,
    const MiniGLM::mat4& projection)
    : model_(model), view_(view), projection_(projection)
{}

void VertexProcessor::setModelMatrix(const MiniGLM::mat4& model) {
    model_ = model;
}

void VertexProcessor::setViewMatrix(const MiniGLM::mat4& view) {
    view_ = view;
}

void VertexProcessor::setProjectionMatrix(const MiniGLM::mat4& projection) {
    projection_ = projection;
}

std::vector<MiniGLM::vec4> VertexProcessor::transformVertices(
    const std::vector<MiniGLM::vec3>& vertices) const
{
    std::vector<MiniGLM::vec4> transformed;
    transformed.reserve(vertices.size());
    MiniGLM::mat4 mvp = projection_ * view_ * model_; // Column-major multiplication
    for(const auto& v : vertices) {
        MiniGLM::vec4 v4 = MiniGLM::vec4(v, 1.0f); // Homogeneous coordinate
        transformed.push_back(mvp * v4);
    }
    return transformed;
}
