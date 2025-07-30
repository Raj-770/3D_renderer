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

private:
    bool validate_vertex(const std::vector<std::string> tokens);
    bool validate_face(const std::vector<std::string> tokens);
    
};