#include "collision.h"
#include "raylib.h"
#include "storage.h"
#include <assert.h>

Rectangle get_ball_collision_rect(World *world, const Entity entity) {
    Ball *ball = get_ball(&world->ball_storage, entity);
    Position *pos = get_position(&world->position_storage, entity);
    return (Rectangle){.height = ball->radius,
                       .width = ball->radius,
                       .x = pos->x,
                       .y = pos->y};
}

Rectangle *get_all_brick_collision_rects(World *world, Rectangle *rects,
                                         int *count) {
    *count = 0;
    for (size_t i = 0; i < world->brick_storage.dense_count; i++) {
        Entity entity = world->brick_storage.dense_entities[i];
        assert(has_brick(&world->brick_storage, entity));
        rects[*count] = get_brick_collision_rect(world, entity);
        (*count)++;
    }
    return rects;
}

Rectangle get_brick_collision_rect(World *world, const Entity entity) {
    Brick *brick = get_brick(&world->brick_storage, entity);
    Position *pos = get_position(&world->position_storage, entity);
    return (Rectangle){.height = (float)brick->size.height,
                       .width = (float)brick->size.width,
                       .x = pos->x,
                       .y = pos->y};
}

Rectangle get_paddle_collision_rect(World *world) {
    Paddle *paddle = get_paddle(&world->paddle_storage, world->player);
    Position *pos = get_position(&world->position_storage, world->player);
    return (Rectangle){.height = (float)paddle->size.height,
                       .width = (float)paddle->size.width,
                       .x = pos->x,
                       .y = pos->y};
}
