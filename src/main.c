#include "entity.h"
#include "input.h"
#include "render.h"
#include "storage.h"
#include "system.h"
#include "world.h"
#include <assert.h>
#include <raylib.h>
#include <stdio.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 400
#define PADDLE_HEIGHT 5
#define PADDLE_WIDTH 50

Entity init_paddle(World *world) {
  Entity entity = create_entity(&world->entity_manager);
  Position pos = {.x = 30, .y = SCREEN_HEIGHT - 20};
  upsert_position(&world->position_storage, entity, pos);
  Paddle pad = {.size = (Size){.width = PADDLE_WIDTH, .height = PADDLE_HEIGHT},
                .color = BLACK,
                .speed = 10};
  upsert_paddle(&world->paddle_storage, entity, pad);
  return entity;
}

Entity init_ball(World *world) {
  Entity entity = create_entity(&world->entity_manager);
  Position pos = {.x = (float)SCREEN_WIDTH/2, .y = (float)SCREEN_HEIGHT/2};
  upsert_position(&world->position_storage, entity, pos);
  upsert_ball(&world->ball_storage, entity,
              (Ball){.color = BLACK, .radius = 5, .dmg = 1});
  return entity;
}

#define BRICK_WIDTH 80
#define BRICK_HEIGHT 20
#define BRICK_OFFSET 5
#define BRICK_MARGIN 15
#define BRICK_COUNT (SCREEN_WIDTH - (2 * BRICK_MARGIN)) / BRICK_WIDTH

void init_brick(World *world, float x, float y, int hp) {
  Entity entity = create_entity(&world->entity_manager);
  Position pos = {.x = x, .y = y};
  upsert_position(&world->position_storage, entity, pos);
  upsert_brick(
      &world->brick_storage, entity,
      (Brick){.size = (Size){.width = BRICK_WIDTH, .height = BRICK_HEIGHT},
              .color = BLACK, .hp=(uint8_t)hp});
}

void init_bricks(World *world) {
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < BRICK_COUNT; j++) {
      init_brick(
          world, (float)((j * BRICK_WIDTH) + BRICK_MARGIN + (j * BRICK_OFFSET)),
          (float)((i * BRICK_HEIGHT) + (i * BRICK_OFFSET) + BRICK_MARGIN),
          2-i*5
      );
    }
}

int main(void) {
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "ecs-engine");
  SetExitKey(KEY_BACKSPACE);

  World world = init_world();
  world.player = init_paddle(&world);
  init_ball(&world);
  init_bricks(&world);

  assert(init_systems());
  system_add(move_system);
  system_add(debug_system);
  system_add(input_system);

  while (!WindowShouldClose()) {
    { // CORE SYSTEMS
      world.game_state.delta_time = GetFrameTime();
      if (!pool_events(&world.input_state))
        return -1;
    }
    { // ECS Systems
      run_systems(&world);
    }
    { // RENDERING
      BeginDrawing();
      render(&world);
      EndDrawing();
    }
    {
      // EVENT SYSTEM (?)
      // TBI
    }
  }

  CloseWindow();
  destroy_world(&world);
  return 0;
}
