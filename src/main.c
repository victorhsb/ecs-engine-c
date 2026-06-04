#include <assert.h>
#include <raylib.h>
#include "entity.h"
#include "render.h"
#include "storage.h"
#include "world.h"
#include "system.h"
#include "input.h"

Entity init_paddle(World *world) {
    Entity entity = create_entity(&world->entity_manager);
    Position pos = {.x = 30, .y = 400};
    upsert_position(&world->position_storage, entity, pos);
    Paddle pad = {.size = 20, .color = BLACK, .speed = 120};
    upsert_paddle(&world->paddle_storage, entity, pad);
    return entity;
}

Entity init_ball(World *world) {
    Entity entity = create_entity(&world->entity_manager);
    Position pos = {.x = 50, .y = 400};
    upsert_position(&world->position_storage, entity, pos);
    upsert_ball(&world->ball_storage, entity, (Ball){.color=BLACK, .radius=.5, .dmg=1});
    return entity;
}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "ecs-engine");

    assert(init_systems());
    system_add(move_system);

    World world = init_world();
    world.player = init_paddle(&world);
    init_ball(&world);

    while (!WindowShouldClose()) {
        { // CORE SYSTEMS
            world.game_state.delta_time = GetFrameTime();
            if (!pool_events(&world.input_state)) return -1;
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
    return 0;
}
