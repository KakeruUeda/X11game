#pragma once

#include <algorithm>

namespace mygame {

struct Point {
    int x, y;
};

struct Size {
    int width, height;
};

struct Rect {
    int x, y;
    int width, height;

    inline Point tl() const {
        return { std::min(x, x + width), std::min(y, y + height) };
    }

    inline Point br() const {
        return { std::max(x, x + width), std::max(y, y + height) };
    }

    inline Point tr() const {
        return { std::max(x, x + width), std::min(y, y + height) };
    }

    inline Point bl() const {
        return { std::min(x, x + width), std::max(y, y + height) };
    }
};

inline bool pointInRect(const Point &p, const Rect &r) {
    return (   p.x >= r.tl().x && p.x <= r.br().x
            && p.y >= r.tl().y && p.y <= r.br().y);
}

inline bool inRange(int i, int min_i, int max_i) {
    return (i >= min_i && i <= max_i);
}

inline bool rectangleIntersect(const Rect &r1, const Rect &r2) {
    // Check 1 -- Any corner inside rect
    if ((pointInRect(r1.tl(), r2) || pointInRect(r1.br(), r2))
        || (pointInRect(r1.tr(), r2) || pointInRect(r1.bl(), r2)))
        return true;

    // Check 2 -- Overlapped, but all points outside
    //     +---+
    //  +--+---+----+
    //  |  |   |    |
    //  +--+---+----+
    //     +---+

    if ((inRange(r1.tl().x, r2.tl().x, r2.br().x) || inRange(r1.br().x, r2.tl().x, r2.br().x))
            && r1.tl().y < r2.tl().y && r1.br().y > r2.br().y
        || ((inRange(r1.tl().y, r2.tl().y, r2.br().y) || inRange(r1.br().y, r2.tl().y, r2.br().y)))
            && r1.tl().x < r2.tl().x && r1.br().x > r2.br().x)
        return true;
    
    return false;
}

struct Character {
    unsigned long color;
    Point position;
    Size size;

    Character(unsigned long new_col, Point new_pos, Size new_sz)
    : color(new_col), position(new_pos), size(new_sz) {};

    Rect bounds() const {
        return { position.x, position.y, size.width, size.height };
    }
};

struct Player : public Character {
    Player() : Character(0x1077e6, {10, 10}, {10, 10}) {};
};

struct Food : public Character {
    Food() : Character(0xe69810, {10, 10}, {10, 10}) {};
};

struct Ghost : public Character {
    Ghost() : Character(0xa832a8, {10, 10}, {10, 10}) {};
};

}