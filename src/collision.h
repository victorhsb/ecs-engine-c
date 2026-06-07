#pragma once

#include "raylib.h"
#include "world.h"

#define BRICK_ROWS 3
#define BRICK_COLUMNS 9

typedef struct CollisionObject {
    Rectangle rect;
    Entity ent;
} CollisionObject;

typedef struct CollisionWorld {
    CollisionObject paddle;
    CollisionObject ball;
    // sparse array mimicking the shape of the brick grid
    CollisionObject bricks[BRICK_ROWS*BRICK_COLUMNS];
} CollisionWorld;

Rectangle get_brick_collision_rect(World *world, Entity const entity);
Rectangle get_ball_collision_rect(World *world, Entity const entity);
Rectangle get_paddle_collision_rect(World *world);
