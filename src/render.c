#include "render.h"
#include "component.h"
#include "storage.h"

int render(World *world) {
  ClearBackground(RAYWHITE);

  Paddle *paddle = get_paddle(&world->paddle_storage, world->player);
  if (paddle) {
    Position *pos = get_position(&world->position_storage, world->player);
    if (pos) {
      DrawRectangle((int)pos->x, (int)pos->y, paddle->size.width,
                    paddle->size.height, paddle->color);
    }
  }

  if (world->ball_storage.entity != 0) {
    Position *pos =
        get_position(&world->position_storage, world->ball_storage.entity);
    if (pos) {
      DrawCircle((int)pos->x, (int)pos->y, world->ball_storage.data.radius,
                 world->ball_storage.data.color);
    }
  }

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

  DrawText("my first window in C", 190, 200, 20, LIGHTGRAY);
  return 1;
}
