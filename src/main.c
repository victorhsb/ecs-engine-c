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
    upsert_position(&world->position_storage, entity, (Position){0});
    upsert_paddle(&world->paddle_storage, entity, (Paddle){0});
    upsert_ball(&world->ball_storage, entity, (Ball){0});
    return entity;
}

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "ecs-engine");

    assert(init_systems());

    World world = init_world();
    world.player = init_paddle(&world);

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
