#pragma once

#include "InputHandler.hpp"
#include "ObjParser.hpp"
#include "Renderer.hpp"
#include <string>

struct AppContext {
  Renderer *renderer;
  InputHandler *inputHandler;
};

int runRenderer(const ObjParser &parser,
                const std::string &windowTitle = "3D Wireframe Renderer");