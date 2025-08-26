#include <iostream>
#include <string>

#include "Framer.hpp"

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: ./framer <.obj file>\n";
    return 1;
  }

  ObjParser parser;
  if (!parser.load(argv[1])) {
    std::cerr << "Failed to load OBJ file.\n";
    return 1;
  }
  std::cout << "Loaded " << parser.vertices.size() << " vertices and "
            << parser.faces.size() << " faces with " << parser.edges.size()
            << " edges.\n";

  return runRenderer(parser);
}
