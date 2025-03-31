#include <X11/Xlib.h>
#include <cstdio>
#include <stdexcept>

#define KEY_ESCAPE     61
#define KEY_SPACEBAR   57
#define KEY_UP        134
#define KEY_DOWN      133
#define KEY_LEFT      131
#define KEY_RIGHT     132

namespace mygame {

struct Rect {
    long x, y;
    long width, height;
};

class GameDisplay {
public:
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

// RAII -- Resource Acquisitio Is Initialization
GameDisplay::GameDisplay() {
    display_ = XOpenDisplay(NULL);
    if(display_ == NULL) {
        throw std::runtime_error("Unable to open the display\n");
    }
    
    screen_ = DefaultScreen(display_);
    
    window_ = XCreateSimpleWindow(display_, RootWindow(display_, screen_), 0, 0, 100, 100, 1, 
                                    BlackPixel(display_, screen_), WhitePixel(display_, screen_));
    
    
    XSelectInput(display_, window_, KeyPressMask | ExposureMask);
    XMapWindow(display_, window_);
}

GameDisplay::~GameDisplay() {
    XCloseDisplay(display_);
}

Display *GameDisplay::getDisplay() {
    return display_;
}

void GameDisplay::drawRect(unsigned long col, int x, int y, int width, int height) {
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

class Game {
public:
    Game();

    void run();

private:
    GameDisplay gamedisplay_;
    XEvent event_;

    bool is_running_ = true; 
    int x_ = 10;
    int y_ = 10;

    bool getEvent();
    void handleEvent();
    bool isPlayerWithinBounds();

};

Game::Game() {
}

void Game::run() {
    while (is_running_) {
        if (getEvent()){
            handleEvent();
            if (!isPlayerWithinBounds()) {
                printf("PLAYER OUT OF BOUNDS .. GAME OVER!! .. YOU LOSE!!\n");
                is_running_ = false;
            }
        }
    }
}

bool Game::getEvent() {
    if (XPending(gamedisplay_.getDisplay())) {
        XNextEvent(gamedisplay_.getDisplay(), &event_);
        printf("Event: %d\n", event_.type);

        return true; 
    }

    return false;
}

void Game::handleEvent() {
    if(event_.type == Expose) {
        gamedisplay_.drawRect(0x6091ab, x_, y_, 10, 10);
    }
    if(event_.type == KeyPress) {
        printf("KeyPress Event: %d\n", event_.xkey.keycode);
    
        switch (event_.xkey.keycode) {
            case KEY_UP       : printf("KEY_UP\n");    y_ -= 2; gamedisplay_.redraw(); break;
            case KEY_DOWN     : printf("KEY_DOWN\n");  y_ += 2; gamedisplay_.redraw(); break;
            case KEY_LEFT     : printf("KEY_LEFT\n");  x_ -= 2; gamedisplay_.redraw(); break;
            case KEY_RIGHT    : printf("KEY_RIGHT\n"); x_ += 2; gamedisplay_.redraw(); break;
            case KEY_SPACEBAR : printf("KEY_SPACEBAR\n"); break;
            case KEY_ESCAPE   : printf("KEY_ESCAPE\n"); 
                                is_running_ = false; break;
        }
    }
}

bool Game::isPlayerWithinBounds() {
    Rect w = gamedisplay_.getGeometry();
    
    if (x_ < 0 || x_ > w.width || y_ < 0 || y_ > w.height) {
        return false;
    }

    return true;
}

}

int main() {
    mygame::Game game;

    game.run();              

    printf("EXITING\n");
}