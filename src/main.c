#include <assert.h>
#include <stdio.h>
#include <raylib.h>
#include "world.h"
#include "system.h"

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "ecs-engine");

    assert(init_systems());
    system_add(rendering_system);

    World world = init_world();

    while (!WindowShouldClose()) {
        world.game_state.delta_time = GetFrameTime();
        run_systems(&world);
    }

    CloseWindow();
    return 0;
}
