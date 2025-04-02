#include "Game.hpp"
#include "KeyCodes.hpp"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>

namespace mygame {

Game::Game() {
    std::srand(std::time(nullptr));
    createFood();
    createGhosts();
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
    drawAllGhosts();
    drawPlayer();
}

void Game::createFood() {
    food_.clear();
    food_.resize(10);
    
    const int MAXX = 800;
    const int MAXY = 600;

    for (auto &f : food_) {
        f.position.x = (std::rand() % MAXX) / 10 * 10;
        f.position.y = (std::rand() % MAXY) / 10 * 10;
    }
}

void Game::drawAllFood() {
    for (auto &f : food_) {
        drawCharacter(f);
    }
}

void Game::createGhosts() {
    ghosts_.clear();
    ghosts_.resize(10);
    
    const int MAXX = 800;
    const int MAXY = 600;

    for (auto &g : ghosts_) {
        g.position.x = (std::rand() % MAXX) / 10 * 10;
        g.position.y = (std::rand() % MAXY) / 10 * 10;
    }
}

void Game::drawAllGhosts() {
    for (auto &g : ghosts_) {
        drawCharacter(g);
    }
}

void Game::drawCharacter(const Character &obj) const {
    gamedisplay_.drawRect(obj.color, 
        obj.position.x, 
        obj.position.y,
        obj.size.width,
        obj.size.height);
}


void Game::update() {
    auto iter = std::find_if(food_.begin(), food_.end(), [&](const Food &f) {
        return rectangleIntersect(player_.bounds(), f.bounds());
    });

    if (iter != food_.end()) {
        food_.erase(iter);
    }

    if (food_.empty()) {
        is_running_ = false;
        std::cout << "YOU LOSE!!\n";
    }

	auto iter_ghosts = std::find_if(ghosts_.begin(), ghosts_.end(), [&](const Ghost &g){
		return rectangleIntersect(player_.bounds(), g.bounds());
	});

	if (iter_ghosts != ghosts_.end())
	{
		is_running_ = false;
		std::cout << "YOU LOSE!!\n";
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
        update();
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