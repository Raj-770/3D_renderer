#include "InputHandler.hpp"
#include "MiniFB.h"

InputHandler::InputHandler()
    : scrollY_(0.0f), lastMouseX_(0), lastMouseY_(0),
      deltaX_(0.0f), deltaY_(0.0f), rotating_(false)
{}

bool InputHandler::update(struct mfb_window *window) {
    bool changed = false;

    float tempScroll = mfb_get_mouse_scroll_y(window);
    scrollY_ = tempScroll;
    if (tempScroll != 0.0f)
        changed = true;

    const uint8_t *buttons = mfb_get_mouse_button_buffer(window);
    bool rightDown = (buttons[1] == 1);

    int mouseX = mfb_get_mouse_x(window);
    int mouseY = mfb_get_mouse_y(window);

    if (rightDown) {
        if (!rotating_) {
            lastMouseX_ = mouseX;
            lastMouseY_ = mouseY;
            deltaX_ = 0.0f;
            deltaY_ = 0.0f;
            rotating_ = true;
        } else {
            deltaX_ = float(mouseX - lastMouseX_);
            deltaY_ = float(mouseY - lastMouseY_);
            lastMouseX_ = mouseX;
            lastMouseY_ = mouseY;
            if (deltaX_ != 0.0f || deltaY_ != 0.0f)
                changed = true;
        }
    } else {
        rotating_ = false;
        deltaX_ = 0.0f;
        deltaY_ = 0.0f;
    }

    return changed;
}

float InputHandler::getScrollY() const { return scrollY_; }
bool InputHandler::isRotating() const { return rotating_; }
float InputHandler::getDeltaX() const { return deltaX_; }
float InputHandler::getDeltaY() const { return deltaY_; }
