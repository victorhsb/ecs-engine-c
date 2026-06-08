#pragma once

#include "raylib.h"
#include "world.h"

#define BRICK_ROWS 3
#define BRICK_COLUMNS 9

typedef struct CollisionObject {
    Rectangle rect;
    Entity ent;
} CollisionObject;

/*
 * Collision system plan:
 * A first pass does straight calculations to check if the paddle or ball are crossing the walls
 * then a second pass does a broad calculation to validate ball<>brick collision which
 * happens at a grid level (we can filter which bricks are nearby roughly to reduce calculations maybe(?))
 */

/* The full scope of the collision system. should we cache it? should we recalculate it every time? */
typedef struct CollisionWorld {
    CollisionObject paddle;
    CollisionObject ball;
    // sparse array mimicking the shape of the brick grid
    CollisionObject bricks[BRICK_ROWS*BRICK_COLUMNS];
} CollisionWorld;

Rectangle get_brick_collision_rect(World *world, Entity const entity);
Rectangle get_ball_collision_rect(World *world, Entity const entity);
Rectangle get_paddle_collision_rect(World *world);
