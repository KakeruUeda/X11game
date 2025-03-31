#pragma once

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

}