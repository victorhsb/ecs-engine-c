#include "world.h"
#include "entity.h"
#include "storage.h"
#include <stdint.h>
#include <stdlib.h>


GameState GameState_init(void) {
    return (GameState){0};
}

World init_world(void) {
    World world = {0};
    // ecs
    world.position_storage = PositionStorage_init();
    world.velocity_storage = VelocityStorage_init();
    world.paddle_storage = PaddleStorage_init();
    world.ball_storage = BallStorage_init();
    world.brick_storage = BrickStorage_init();

    // core engine
    world.entity_manager = init_entity_manager();
    world.input_state = InputState_init();
    world.game_state = GameState_init();

    return world;
}

void destroy_world(World *world) {
    PositionStorage_destroy(&world->position_storage);
    VelocityStorage_destroy(&world->velocity_storage);
    BrickStorage_destroy(&world->brick_storage);

    destroy_entity_manager(&world->entity_manager);
    free(world);
}
