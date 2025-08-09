#include "VertexProcessor.hpp"

VertexProcessor::VertexProcessor() 
    : mvpMatrix_(MiniGLM::mat4(1.0f)), matricesSet_(false) {
}

void VertexProcessor::setMatrices(const ModelMatrix& model, const ViewMatrix& view, 
                                  const ProjectionMatrix& projection) {
    mvpMatrix_ = projection.getMatrix() * view.getViewMatrix() * model.getModelMatrix();
    matricesSet_ = true;
}

std::vector<MiniGLM::vec4> VertexProcessor::transformVertices(
    const std::vector<MiniGLM::vec3>& vertices) const {
    
    std::vector<MiniGLM::vec4> clipVertices;
    clipVertices.reserve(vertices.size());
    
    for (const auto& vertex : vertices) {
        MiniGLM::vec4 homogeneousVertex(vertex.x, vertex.y, vertex.z, 1.0f);
        
        MiniGLM::vec4 clipVertex = mvpMatrix_ * homogeneousVertex;
        clipVertices.push_back(clipVertex);
    }
    
    return clipVertices;
}

std::vector<MiniGLM::vec3> VertexProcessor::clipToNDC(
    const std::vector<MiniGLM::vec4>& clipVertices) const {
    
    std::vector<MiniGLM::vec3> ndcVertices;
    ndcVertices.reserve(clipVertices.size());
    
    for (const auto& clipVertex : clipVertices) {
        if (clipVertex.w != 0.0f) {
            float invW = 1.0f / clipVertex.w;
            MiniGLM::vec3 ndcVertex(
                clipVertex.x * invW,
                clipVertex.y * invW,
                clipVertex.z * invW
            );
            ndcVertices.push_back(ndcVertex);
        } else {
            ndcVertices.push_back(MiniGLM::vec3(0.0f, 0.0f, 0.0f));
        }
    }
    
    return ndcVertices;
}

std::vector<MiniGLM::vec2> VertexProcessor::ndcToScreen(
    const std::vector<MiniGLM::vec3>& ndcVertices, 
    int screenWidth, int screenHeight) const {
    
    std::vector<MiniGLM::vec2> screenVertices;
    screenVertices.reserve(ndcVertices.size());
    
    float halfWidth = screenWidth * 0.5f;
    float halfHeight = screenHeight * 0.5f;
    
    for (const auto& ndcVertex : ndcVertices) {
        float screenX = (ndcVertex.x + 1.0f) * halfWidth;
        float screenY = (1.0f - ndcVertex.y) * halfHeight;
        
        screenVertices.push_back(MiniGLM::vec2(screenX, screenY));
    }
    
    return screenVertices;
}

std::vector<MiniGLM::vec2> VertexProcessor::processVertices(
    const std::vector<MiniGLM::vec3>& vertices,
    int screenWidth, int screenHeight) const {
    
    if (!matricesSet_) {
        return std::vector<MiniGLM::vec2>();
    }
    
    auto clipVertices = transformVertices(vertices);
    auto ndcVertices = clipToNDC(clipVertices);
    auto screenVertices = ndcToScreen(ndcVertices, screenWidth, screenHeight);
    
    return screenVertices;
}

MiniGLM::mat4 VertexProcessor::getMVPMatrix() const {
    return mvpMatrix_;
}

bool VertexProcessor::isReady() const {
    return matricesSet_;
}

void VertexProcessor::updateMVPMatrix() {
    // This method could be used if you want to update the MVP matrix
    // without calling setMatrices again, but would require storing
    // references to the individual matrices
}
