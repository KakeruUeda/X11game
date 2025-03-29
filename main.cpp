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

class GameDisplay {
public:
    GameDisplay();
    ~GameDisplay();

    Display* getDisplay();

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
    
    
    XSelectInput(display_, window_, KeyPressMask);
    XMapWindow(display_, window_);
}

GameDisplay::~GameDisplay() {
    XCloseDisplay(display_);
}

Display *GameDisplay::getDisplay() {
    return display_;
}

class Game {
public:
    Game();

    void run();

private:
    GameDisplay gamedisplay_;
    XEvent event_;

    bool is_running_ = true; 

    bool getEvent();
    void handleEvent();
};

Game::Game() {
}

void Game::run() {
    while (is_running_) {
        if (getEvent()){
            handleEvent();
        };
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
    if(event_.type == KeyPress) {
        printf("KeyPress Event: %d\n", event_.xkey.keycode);
    
        switch (event_.xkey.keycode) {
            case KEY_UP       : printf("KEY_UP\n"); break;
            case KEY_DOWN     : printf("KEY_DOWN\n"); break;
            case KEY_LEFT     : printf("KEY_LEFT\n"); break;
            case KEY_RIGHT    : printf("KEY_RIGHT\n"); break;
            case KEY_SPACEBAR : printf("KEYY__SPACEBAR\n"); break;
            case KEY_ESCAPE   : printf("KEY_ESCAPE\n"); 
                                is_running_ = false; break;
        }
    }
}

}

int main() {
    mygame::Game game;

    game.run();              

    printf("EXITING\n");
}