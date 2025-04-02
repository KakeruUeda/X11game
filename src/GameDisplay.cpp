#include "GameDisplay.hpp"
#include <stdexcept>
#include <cstdio>

namespace mygame {

GameDisplay::GameDisplay() {
    display_ = XOpenDisplay(NULL);
    if (display_ == NULL) {
        throw std::runtime_error("Unable to open the display\n");
    }

    screen_ = DefaultScreen(display_);

    window_ = XCreateSimpleWindow(display_, RootWindow(display_, screen_), 0, 0,
                                  DEFAULT_WIDTH, DEFAULT_HEIGHT, 1,
                                  BlackPixel(display_, screen_), 0x363d4d);

    XSelectInput(display_, window_, KeyPressMask | ExposureMask);
    XMapWindow(display_, window_);
}

GameDisplay::~GameDisplay() {
    XCloseDisplay(display_);
}

Display *GameDisplay::getDisplay() {
    return display_;
}

void GameDisplay::drawRect(unsigned long col, int x, int y, int width, int height) const {
    XSetForeground(display_, DefaultGC(display_, screen_), col);
    XFillRectangle(display_, window_, DefaultGC(display_, screen_), x, y, width, height);
}

void GameDisplay::redraw() {
    XClearWindow(display_, window_);
    Window root_wind;
    int x, y;
    unsigned int width, height, border_width, depth;
    XGetGeometry(display_, window_, &root_wind, &x, &y, &width,
                    &height, &border_width, &depth);
    XEvent ev;
    ev.xexpose.type = Expose;
    ev.xexpose.display = display_;
    ev.xexpose.window = window_;
    ev.xexpose.x = x;
    ev.xexpose.y = y;
    ev.xexpose.width = width;
    ev.xexpose.height = height;
    ev.xexpose.count = 0;

    XSendEvent(display_, window_, false, ExposureMask, &ev);
}

Rect GameDisplay::getGeometry() {
    Window root_wind;
    int x, y;
    unsigned int width, height, border_width, depth;
    XGetGeometry(display_, window_, &root_wind, &x, &y, &width,
                    &height, &border_width, &depth);

    Rect r;

    r.x = x;
    r.y = y;
    r.width = width;
    r.height = height;

    return r;
}

}