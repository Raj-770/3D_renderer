#include "ObjParser.hpp"

#include <fstream>
#include <sstream>

bool ObjParser::load(const std::string& filename) {
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
                vertices.push_back({std::stof(tokens[0]), std::stof(tokens[1]), std::stof(tokens[2])});
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
                faces.push_back(face);
            }
        }
    }
    return true;
}

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

bool ObjParser::validate_face(const std::vector<std::string> tokens) {
    for (const auto& token : tokens) {
        const auto slash = token.find('/');
        std::string idx = (slash == std::string::npos) ? token : token.substr(0, slash);
        if (idx.empty() || idx.find_first_not_of("0123456789") != std::string::npos)
            return false;
    }
    return true;
}