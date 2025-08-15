#pragma once
#include "MiniGLM.hpp"

class Clipper {
public:
    // Screen bounds constructor (for screen-space clipping)
    Clipper(int xmin, int ymin, int xmax, int ymax);
    
    // 3D clipping constructor (for near plane clipping)
    Clipper(float nearPlane);
    
    // Screen space line clipping (existing functionality)
    bool clipLine(MiniGLM::ivec2& p0, MiniGLM::ivec2& p1) const;
    
    // 3D line clipping against near plane in clip space
    bool clipLineNearPlane(MiniGLM::vec4& p0, MiniGLM::vec4& p1) const;
    
private:
    // Screen clipping bounds
    int xmin_, ymin_, xmax_, ymax_;
    
    // Near plane distance (in clip space, this is w = nearPlane)
    float nearPlane_;
    
    // Screen clipping mode flag
    // bool useScreenClipping_;
    
    enum OutCode {
        INSIDE = 0,  // 0000
        LEFT   = 1,  // 0001
        RIGHT  = 2,  // 0010
        BOTTOM = 4,  // 0100
        TOP    = 8   // 1000
    };

    int computeOutCode(int x, int y) const;
    
    // Linearly interpolate between two vec4 points
    MiniGLM::vec4 lerp(const MiniGLM::vec4& a, const MiniGLM::vec4& b, float t) const;
};
