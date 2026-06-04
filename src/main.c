#include <assert.h>
#include <stdio.h>
#include <raylib.h>
#include "render.h"
#include "world.h"
#include "system.h"
#include "input.h"

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "ecs-engine");

    assert(init_systems());
    // system_add(rendering_system);

    World world = init_world();

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
