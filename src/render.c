#include "render.h"
#include "component.h"
#include "storage.h"
#include "world.h"
#include <assert.h>
#include <raylib.h>
#include <stdio.h>

static void render_paddle(World *world) {
    Paddle *paddle = get_paddle(&world->paddle_storage, world->player);
    Position *pos = get_position(&world->position_storage, world->player);
    assert(pos != NULL);
    assert(paddle != NULL);
    DrawRectangle((int)pos->x, (int)pos->y, paddle->size.width,
                  paddle->size.height, paddle->color);
}

static void render_ball(World *world) {
    if (world->ball_storage.entity == 0)
        return;
    Position *pos =
        get_position(&world->position_storage, world->ball_storage.entity);
    if (pos) {
        DrawCircle((int)pos->x, (int)pos->y, world->ball_storage.data.radius,
                   world->ball_storage.data.color);
    }
}

static void render_bricks(World *world) {
    for (int i = 0; i <= (int)world->brick_storage.dense_count; i++) {
        Entity ent = world->brick_storage.dense_entities[i];
        Brick *brick = get_brick(&world->brick_storage, ent);
        if (brick) {
            Position *pos = get_position(&world->position_storage, ent);
            if (pos)
                DrawRectangle((int)pos->x, (int)pos->y, brick->size.width,
                              brick->size.height, get_brick_color(*brick));
        }
    }
}

int render(World *world) {
    ClearBackground(RAYWHITE);

    render_paddle(world);
    render_ball(world);
    render_bricks(world);

    if (world->game_state.paused) {
        DrawText("Paused, press space to continue", 100, 200, 20, BLACK);
    }

    return 1;
}
