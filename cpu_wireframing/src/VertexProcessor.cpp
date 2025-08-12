#include "VertexProcessor.hpp"

/**
 * @brief Construct a VertexProcessor with given model, view, and projection matrix.
 * 
 * @param model The model (object-to-world) transformation matrix
 * @param view The view (world-to-camera) transformation matrix
 * @param projection The projection (camera-to-clip-space) matrix
 */
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

/**
 * @brief Applies the Model-View-Projection (MVP) trasformation to a list of vertices.
 *        It takes a collection of object-space vertices, converts each into a homogeneous
 *        coordinate (vec4) with w=1.0, and multiplies it by the combined MVP matrix
 *        to obtain the transformed vertices in clip space.
 * 
 * @param vertices The input vertices in object space as a vector of Vec3
 * @return std::vector<MiniGLM::vec4>  A vector containing the transformed vertices in clip space.
 */
std::vector<MiniGLM::vec4> VertexProcessor::transformVertices(
    const std::vector<MiniGLM::vec3>& vertices) const
{
    std::vector<MiniGLM::vec4> transformed;
    transformed.reserve(vertices.size());
    MiniGLM::mat4 mvp = projection_ * view_ * model_;
    for(const auto& v : vertices) {
        MiniGLM::vec4 v4 = MiniGLM::vec4(v, 1.0f);
        transformed.push_back(mvp * v4);
    }
    return transformed;
}
