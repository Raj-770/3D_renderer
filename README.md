# 3D Wireframe Renderer

This project implements a 3D wireframe renderer supporting OBJ files, featuring both a CPU-based and a GPU-accelerated rendering pipeline. The CPU version demonstrates a multithreaded rasterization approach, while the GPU version leverages modern OpenGL for efficient rendering.

---

## Project Structure

```
root/
│
├── cpu_wireframing/      # CPU version source code
├── gpu_wireframing/      # GPU version source code
├── Makefile
│
└── ...                   # Other files
```

---

## CPU Wireframe Renderer

- **Location:** `cpu_wireframing/`
- **Build:** In the project root, simply run:
  ```
  make
  ```
- **Output:**

  - `render_gui` – Interactive renderer with a GUI window (uses minifb for framebuffer handling).
  - `render_to_file` – Offscreen renderer that outputs `.png` images (uses `stb_image_writer.h`).

- **Key Features:**
  - Custom multithreaded rasterizer for fast, software-based rendering.
  - minifb library for framebuffer management and window creation.
  - Straightforward pipeline for both real-time GUI and file-based rendering.

---

## GPU Wireframe Renderer

- **Location:** `gpu_wireframing/`
- **Build:** In the project root, run:
  ```
  make gpu
  ```
- **Output:**

  - `gpu_wireframe` – Single executable for GPU-accelerated wireframe rendering.

- **Key Features:**
  - Uses OpenGL for hardware-accelerated rendering.
  - Supports the same input models as the CPU version.

---

## Getting Started

1. **Build the CPU renderer**:

   ```
   make
   ```

   - Use `./render_gui` for an interactive demo.
   - Use `./render_to_file` to export a PNG image.

2. **Build the GPU renderer**:
   ```
   make gpu
   ```
   - Use `./gpu_wireframe` to launch the OpenGL-rendered wireframe viewer.

---

## Dependencies

- **CPU Version:**
  - `stb_image_writer.h` (included): For image output.
  - `minifb` (included or submodule): For window/framebuffer operations.
- **GPU Version:**
  - OpenGL development libraries (platform-specific installation may be required).

---

## Notes

- The CPU version demonstrates custom software rendering techniques and efficient multithreaded drawing.
- The GPU version follows standard OpenGL practices for model-view transformations and rendering pipeline setup.
- Both versions are designed for clarity and educational value in understanding the differences between CPU and GPU graphics processing.

---

Feel free to explore both implementations to see software and hardware approaches to 3D wireframe rendering!
