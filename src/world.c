#include "world.h"
#include "entity.h"
#include "storage.h"
#include <stdint.h>
#include <stdlib.h>


World init_world(void) {
    World world = {0};
    world.position_storage = PositionStorage_init();
    world.velocity_storage = VelocityStorage_init();
    world.paddle_storage = PaddleStorage_init();
    world.ball_storage = BallStorage_init();
    world.brick_storage = BrickStorage_init();

    world.entity_manager = init_entity_manager();

    return world;
}

void destroy_world(World *world) {
    PositionStorage_destroy(&world->position_storage);
    VelocityStorage_destroy(&world->velocity_storage);
    PaddleStorage_destroy(&world->paddle_storage);
    BallStorage_destroy(&world->ball_storage);
    BrickStorage_destroy(&world->brick_storage);

    destroy_entity_manager(&world->entity_manager);
    free(world);
}
