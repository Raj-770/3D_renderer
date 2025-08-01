#include "ObjParser.hpp"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>
#include <iostream>

/**
 * @brief Function to load and store data from the obj file and extract edges.
 * 
 * @param filename .obj filename
 * @return true 
 * @return false 
 */
bool ObjParser::load(const std::string& filename) {
    size_t lastDot = filename.find_last_of('.');
    if (lastDot == std::string::npos || filename.substr(lastDot) != ".obj") {
        std::cerr << "Error: File must have .obj extension. Provided: " << filename << std::endl;
        return false;
    }

    std::ifstream infile(filename);
    if (!infile) return false;

    std::string line;
    while (std::getline(infile, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            std::vector<std::string> tokens;
            std::string temp;
            while (iss >> temp) tokens.push_back(temp);
            if (validate_vertex(tokens)) {
                vertices.push_back(glm::vec3(std::stof(tokens[0]), std::stof(tokens[1]), std::stof(tokens[2])));
            }
        } else if (prefix == "f") {
            std::vector<std::string> tokens;
            std::string temp;
            while (iss >> temp) tokens.push_back(temp);
            if (validate_face(tokens)) {
                Face face;
                for (const auto& vert : tokens) {
                    size_t slash = vert.find('/');
                    int idx = std::stoi(vert.substr(0, slash)) - 1;
                    face.vertex_indices.push_back(idx);
                }
                if (is_valid_face_indices(face.vertex_indices)) {
                    faces.push_back(face);
                } else {
                    std::cerr << "Warning: Face references nonexistent vertex in line: " << line << std::endl;
                }
            }
        }
    }
    extract_edges();
    return true;
}

/**
 * @brief Checks wether a vertex line has three floating point coordinates.
 * 
 * @param tokens 
 * @return true 
 * @return false when the token is anything other than a float
 */
bool ObjParser::validate_vertex(const std::vector<std::string> tokens) {
    if (tokens.size() != 3) return false;
    try {
        for (const auto& t : tokens) {
            std::stof(t);
        }
        return true;
    } catch (...) {
        return false;
    }
}

/**
 * @brief checks if the face line has at least three interger indices and no other characters.
 * 
 * @param tokens 
 * @return true if valid
 * @return false if not valid
 */
bool ObjParser::validate_face(const std::vector<std::string> tokens) {
    if (tokens.size() < 3) {
        return false;
    }

    for (const auto& token : tokens) {
        const auto slash = token.find('/');
        std::string idx = (slash == std::string::npos) ? token : token.substr(0, slash);
        if (idx.empty() || idx.find_first_not_of("0123456789") != std::string::npos)
            return false;
    }
    return true;
}

/**
 * @brief simply checks if the indices in the face exist. 
 * 
 * @param indices 
 * @return true 
 * @return false 
 */
bool ObjParser::is_valid_face_indices(const std::vector<int>& indices) {
    for (int idx : indices) {
        if (idx < 0 || idx >= static_cast<int>(vertices.size())) {
            return false;
        }
    }
    return true;
}

/**
 * @brief find al unique edges using set and store them in the edges variable
 * 
 */
void ObjParser::extract_edges() {
    edges.clear();
    std::set<std::pair<int, int>> uniqueEdges;

    for (const auto& face : faces) {
        int n = face.vertex_indices.size();
        for (int i = 0; i < n; ++i) {
            int v1 = face.vertex_indices[i];
            int v2 = face.vertex_indices[(i + 1) % n];
            // Store edges in sorted order to avoid duplicates (1,3) == (3,1)
            int minV = std::min(v1, v2);
            int maxV = std::max(v1, v2);
            uniqueEdges.insert({minV, maxV});
        }
    }

    for (const auto& e : uniqueEdges) {
        edges.push_back(e);
    }
}

