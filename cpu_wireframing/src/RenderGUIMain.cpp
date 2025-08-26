#include "ObjParser.hpp"
#include "WireframeApp.hpp"
#include <QApplication>
#include <iostream>

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

  QApplication app(argc, argv);

  WireframeApp window(parser.vertices, parser.edges, 1200, 800);

  window.setWindowTitle("Wireframe Renderer");
  window.resize(1200, 800);

  window.show();

  return app.exec();
}
