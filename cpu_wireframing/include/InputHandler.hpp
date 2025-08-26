#pragma once

struct mfb_window;

class InputHandler {
public:
  InputHandler();

  bool updateZoom(struct mfb_window *window);
  void updateRotation(struct mfb_window *window);

  float getScrollY() const;

  bool isRotating() const;

  float getDeltaX() const;
  float getDeltaY() const;

private:
  float scrollY_;
  int lastMouseX_;
  int lastMouseY_;
  float deltaX_;
  float deltaY_;
  bool rotating_;
};
