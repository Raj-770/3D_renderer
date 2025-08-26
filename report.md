# Report

### 3D Wireframe Renderer

Rajkumar Pambhar  
20th August, 2025

## Abstract

This report documents the architecture and implementation of a modular CPU-based wireframe renderer supporting both GUI and command-line operation.

## 1. Core Renderer Implementation

The core of the renderer is structured around several reusable C++ classes:

- **MiniGLM:** Implements custom GLM-like functions and data structures used for storing data and performing mathematical operations needed by the renderer.
- **Obj Parser:** Efficiently parses vertices and faces from OBJ files and creates edges. It is optimized for minimal memory usage by storing only the essential data for wireframe rendering, while robust checks maintain data integrity.
- **Vertex Processor:** Handles the Model-View transformation of all input vertices using composite model, view, and projection matrices. This module leverages multi-threading to accelerate computations and relies on well-structured matrix classes.
- **Rasterizer:** Uses a high-performance, multi-threaded implementation of Xiaolin Wu’s line algorithm for drawing wireframe edges. The pixel buffer is allocated as a contiguous memory block to optimize cache usage and drawing speed.

## 2. GUI Architecture

The GUI version of the application is implemented with Qt 6, a robust cross-platform framework for C++ GUI development.  
Application logic is organized within the `WireframeApp` class, which orchestrates parsing, rendering, and user input handling.

Input is managed either directly through Qt's event mechanism or by a dedicated `InputHandler` class layered atop Qt events, responsible for monitoring mouse movement and scroll events.  
This approach enables camera orbiting and zoom operations: mouse drags adjust yaw and pitch, while the scroll wheel manages zoom by modifying the camera’s distance from the rendered object.

This separation between input processing and rendering logic increases modularity and simplifies code maintenance.

## 3. Exporting Rendered Image

For outputting rendered images to PNG files, the application utilizes **Qt 6's `QImage` class**.  
By creating a `QImage` from the internal framebuffer (using the `Format_RGBA8888` or similar pixel format), the program can save the rendered output directly to disk using the `QImage::save()` function, which supports a variety of image formats including PNG.

The PNG export leverages the same rendering pipeline as the GUI, ensuring output images are consistent with on-screen renders.

## 4. Software Design Justification

By isolating core rendering functionality from user interface components, the renderer remains adaptable for future expansion or alternate front-ends. Input logic is abstracted for ease of testing and maintainability of interactive features.

The renderer’s use of multithreading exploits modern multicore CPUs to achieve fast and fluid frame rates, even for complex models.

Usage of lightweight and task-specific external libraries, with Qt 6 providing the GUI and image export capabilities, results in minimal dependency overhead. Modern C++ features play a central role:

- Resource management follows RAII principles, with smart pointers used where appropriate.
- Strict compilation flags (`-Werror`, `-Wextra`, `-Wall`) uphold code quality.
- Threading (`std::thread`) and concise parallel loops boost performance on multicore CPUs.
- Scoped enumerations, lambdas, range-based for-loops and template instantiation all enhance readability and modularity.

The modular architecture and emphasis on efficient, modern C++ practices ensure the renderer is both fast and maintainable for diverse applications. Its clear separation of rendering, input, and export logic enables easy extensibility and reliable performance across different usage scenarios.
