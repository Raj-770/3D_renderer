#include "InputHandler.hpp"
#include "MiniFB.h"

InputHandler::InputHandler()
    : scrollY_(0.0f)
{}

bool InputHandler::update(struct mfb_window* window) {
    float temp = mfb_get_mouse_scroll_y(window);
    if (scrollY_ != temp) {
        scrollY_ = temp;
        return true;
    }
    return false;
}

float InputHandler::getScrollY() const {
    return scrollY_;
}
