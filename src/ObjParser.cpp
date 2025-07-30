#include "ObjParser.hpp"

#include <fstream>
#include <sstream>

bool ObjParser::load(const std::string& filename) {
    // Generating a stream of the obj file
    std::ifstream infile(filename);
    if (!infile)
        return false;
    
    std::string line;
    
    // Processing the file line by line
    while(std::getline(infile, line)) {
        // Creating a string stream of the line.
        std::istringstream iss(line);

        // Putting everything before the first space into prefix (v or f)
        std::string prefix;
        iss >> prefix;

        // Storing the data according to its prefix (type)
        if (prefix == "v") {
            float x, y, z;
            iss >> x >> y >> z;
            vertices.push_back({x, y, z});
        } else if (prefix == "f") {
            Face face;
            std::string vert;
            while (iss >> vert) {
                size_t slash = vert.find('/');
                int idx = std::stoi(vert.substr(0, slash)) - 1;
                face.vertex_indices.push_back(idx);
            }
            faces.push_back(face);
        }
    }
    return true;
}