#pragma once

#include <string>
#include <vector>
#include <utility>
#include <glm/glm.hpp>

struct Face {
    std::vector<int> vertex_indices;
};

class ObjParser {
public:
    std::vector<glm::vec3> vertices;
    std::vector<Face> faces;
    std::vector<std::pair<int, int>> edges;

    bool load(const std::string& filename);

private:
    bool validate_vertex(const std::vector<std::string> tokens);
    bool validate_face(const std::vector<std::string> tokens);
    bool is_valid_face_indices(const std::vector<int>& indices);
    void extract_edges();

};