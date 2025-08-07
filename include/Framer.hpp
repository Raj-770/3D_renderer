#pragma once

#include "ObjParser.hpp"
#include "Renderer.hpp"
#include "InputHandler.hpp"
#include <string>

struct AppContext {
    Renderer* renderer;
    InputHandler* inputHandler;
};

int runRenderer(const ObjParser& parser, const std::string& windowTitle = "3D Wireframe Renderer");