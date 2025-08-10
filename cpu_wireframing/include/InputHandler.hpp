#pragma once

struct mfb_window;

class InputHandler {
public:
    InputHandler();

    bool update(struct mfb_window* window);

    float getScrollY() const;


private:
    float scrollY_;
};
