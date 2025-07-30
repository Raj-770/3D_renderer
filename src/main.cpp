#include "Framer.hpp"

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Usage: ./framer <.obj file>";
        return 1;
    }
    
    ObjParser parser;
    if (parser.load(argv[1])) {
        std::cout << "Loaded " << parser.vertices.size() << " vertices and " << parser.faces.size() << " faces." << std::endl;
    } else {
        std::cerr << "Failed to load OBJ file." << std::endl; 
    }

    return 0;
}
