#pragma once

#include <string>
#include <vector>

struct Vec3 {
    float x, y, z;
};

struct Face {
    std::vector<int> vertex_indices;
};

class ObjParser {
public:
    std::vector<Vec3> vertices;
    std::vector<Face> faces;

    bool load(const std::string& filename);
};