#include "render.h"
#include "storage.h"

#define PADDLE_HEIGHT 5

int render(World *world) {
  ClearBackground(RAYWHITE);

  Paddle *paddle = get_paddle(&world->paddle_storage, world->player);
  if (paddle) {
      Position *pos = get_position(&world->position_storage, world->player);
      if (pos) {
          DrawRectangle((int)pos->x, (int)pos->y, (int)paddle->size, PADDLE_HEIGHT, paddle->color);
      }
  }

  DrawText("my first window in C", 190, 200, 20, LIGHTGRAY);
  return 1;
}
