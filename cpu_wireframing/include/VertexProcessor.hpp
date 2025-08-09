#pragma once

#include <vector>
#include "MiniGLM.hpp"
#include "ModelMatrix.hpp"
#include "ViewMatrix.hpp"
#include "ProjectionMatrix.hpp"

class VertexProcessor {
public:
    VertexProcessor();
    
    void setMatrices(const ModelMatrix& model, const ViewMatrix& view, const ProjectionMatrix& projection);
    
    std::vector<MiniGLM::vec4> transformVertices(const std::vector<MiniGLM::vec3>& vertices) const;
    
    std::vector<MiniGLM::vec3> clipToNDC(const std::vector<MiniGLM::vec4>& clipVertices) const;
    
    std::vector<MiniGLM::vec2> ndcToScreen(const std::vector<MiniGLM::vec3>& ndcVertices, int screenWidth, int screenHeight) const;
    
    std::vector<MiniGLM::vec2> processVertices(const std::vector<MiniGLM::vec3>& vertices, int screenWidth, int screenHeight) const;
    
    MiniGLM::mat4 getMVPMatrix() const;
    
    bool isReady() const;

private:
    MiniGLM::mat4 mvpMatrix_;
    bool matricesSet_;
    
    void updateMVPMatrix();
};
