#pragma once

#include "raylib.h"
#include <stdint.h>

typedef struct Position {
    float x, y;
} Position;

typedef struct Size {
    int height, width;
} Size;

typedef struct Brick {
    Color color;
    Size size;
    uint8_t hp;
} Brick;
Color get_brick_color(const Brick brick);

typedef struct Velocity {
    float x;
    float y;
} Velocity;

typedef struct Paddle {
    Size size;
    Color color;
    int speed;
} Paddle;

typedef struct Ball {
    float radius;
    Color color;
    uint8_t dmg;
} Ball;
