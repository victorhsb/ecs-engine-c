#pragma once

#include "raylib.h"
#include <stdint.h>

typedef struct Position {
    int x, y;
} Position;

typedef struct Size {
    int height, width;
} Size;

typedef struct Brick {
    Color color;
    Size size;
} Brick;

typedef struct Velocity {
    float x;
    float y;
} Velocity;

typedef struct Paddle {
    uint32_t size;
    Color color;
} Paddle;

typedef struct Ball {
    float radius;
    Color color;
} Ball;
