#pragma once

#include <X11/Xlib.h>
#include "Entities.hpp"

namespace mygame {

struct Rect;

class GameDisplay {
public:
    const int DEFAULT_WIDTH = 800;
    const int DEFAULT_HEIGHT = 600;
    GameDisplay();
    ~GameDisplay();

    Display* getDisplay();
    void drawRect(unsigned long col, int x, int y, int width, int height);
    void redraw();
    Rect getGeometry();

private:
    Display *display_;
    int screen_;
    Window window_;
};

}