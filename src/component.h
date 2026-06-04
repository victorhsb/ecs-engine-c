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

typedef struct Velocity {
    float x;
    float y;
} Velocity;

typedef struct Paddle {
    uint32_t size;
    Color color;
    int speed;
} Paddle;

typedef struct Ball {
    float radius;
    Color color;
    uint8_t dmg;
} Ball;
