#include "render.h"

int render(World *world) {
  (void)world; // just to ignore right now. will be used with proper implementation.
  ClearBackground(RAYWHITE);

  DrawText("my first window in C", 190, 200, 20, LIGHTGRAY);
  return 1;
}
