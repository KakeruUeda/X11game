#pragma once

#include <X11/Xlib.h>
#include <cstdio>
#include <vector>
#include "GameDisplay.hpp"
#include "Entities.hpp"

namespace mygame {

class Game {
public:
    Game();
    void run();

private:
    GameDisplay gamedisplay_;
    XEvent event_;
    Player player_;
    std::vector<Food> food_;
    std::vector<Ghost> ghosts_;
    bool is_running_ = true;

    bool getEvent();
    void handleEvent();
    bool isPlayerWithinBounds();
    void drawPlayer();
    void draw();
    void createFood();
    void drawAllFood();
    void createGhosts();
    void drawAllGhosts();
    void drawCharacter(const Character &obj) const;
    void update();
};

}