/*!
 * X11game
 *
 * Copyright (c) 2025
 *
 * Released under the MIT license.
 * see https://opensource.org/licenses/MIT
 */

#include "Game.hpp"

int main() {
    mygame::Game game;
    game.run();
    
    printf("EXITING\n");
}

/*

 #include <X11/Xlib.h>
 #include <cstdio>
 #include <stdexcept>
 #include <vector>
 #include <cstdlib>
 #include <ctime>
 
 #define KEY_ESCAPE     61
 #define KEY_SPACEBAR   57
 #define KEY_UP        134
 #define KEY_DOWN      133
 #define KEY_LEFT      131
 #define KEY_RIGHT     132
 
 namespace mygame {
 
 struct Point {
     int x, y;
 };
 
 struct Size {
     long width, height;
 };
 
 struct Rect {
     long x, y;
     long width, height;
 };
 
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
 
 // RAII -- Resource Acquisitio Is Initialization
 GameDisplay::GameDisplay() {
     display_ = XOpenDisplay(NULL);
     if(display_ == NULL) {
         throw std::runtime_error("Unable to open the display\n");
     }
     
     screen_ = DefaultScreen(display_);
     
     window_ = XCreateSimpleWindow(display_, RootWindow(display_, screen_), 0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT, 1, 
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
 
 struct Player {
     unsigned long color = 0x1077e6;
     Point position { 10, 10 };
     Size size { 10, 10 };
 };
 
 struct Food {
     unsigned long color = 0xe69810;
     Point position { 100, 100 };
     Size size { 10, 10 };
 };
 
 class Game {
 public:
     Game();
 
     void run();
 
 private:
     GameDisplay gamedisplay_;
     XEvent event_;
 
     Player player_;
     std::vector<Food> food_;
 
     bool is_running_ = true; 
 
     bool getEvent();
     void handleEvent();
     bool isPlayerWithinBounds();
     void drawPlayer();
     void draw();
     void createFood();
     void drawSingleFood(const Food &f);
     void drawAllFood();
 
 };
 
 Game::Game() {
     std::srand(std::time(nullptr));
     createFood();
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
 
 void Game::drawPlayer() {
     gamedisplay_.drawRect(player_.color, 
         player_.position.x, 
         player_.position.y,
         player_.size.width,
         player_.size.height);
 }
 
 void Game::draw() {
     drawAllFood();
     drawPlayer();
 }
 
 void Game::createFood() {
     food_.clear();
     food_.resize(10);
     
     const int MAXX = 800;
     const int MAXY = 600;
 
     for (auto &f : food_) {
         f.position.x = std::rand() % MAXX;
         f.position.y = std::rand() % MAXY;
     }
 }
 
 void Game::drawSingleFood(const Food &f) {
     gamedisplay_.drawRect(f.color, 
         f.position.x, 
         f.position.y,
         f.size.width,
         f.size.height);
 }
 
 void Game::drawAllFood() {
     for (auto &f : food_) {
         drawSingleFood(f);
     }
 }
 
 void Game::handleEvent() {
     if(event_.type == Expose) {
         draw();
     }
     if(event_.type == KeyPress) {
         printf("KeyPress Event: %d\n", event_.xkey.keycode);
     
         switch (event_.xkey.keycode) {
             case KEY_UP       : printf("KEY_UP\n");    player_.position.y -= 10; gamedisplay_.redraw(); break;
             case KEY_DOWN     : printf("KEY_DOWN\n");  player_.position.y += 10; gamedisplay_.redraw(); break;
             case KEY_LEFT     : printf("KEY_LEFT\n");  player_.position.x -= 10; gamedisplay_.redraw(); break;
             case KEY_RIGHT    : printf("KEY_RIGHT\n"); player_.position.x += 10; gamedisplay_.redraw(); break;
             case KEY_SPACEBAR : printf("KEY_SPACEBAR\n"); break;
             case KEY_ESCAPE   : printf("KEY_ESCAPE\n"); 
                                 is_running_ = false; break;
         }
     }
 }
 
 bool Game::isPlayerWithinBounds() {
     Rect w = gamedisplay_.getGeometry();
     
     if (   player_.position.x < 0 || player_.position.x > w.width
         || player_.position.y < 0 || player_.position.y > w.height) {
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
*/