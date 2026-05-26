#include "world.h"
#include <stdlib.h>


World init_world(void) {
    World world = {0};
    world.position_storage = PositionStorage_init();
    world.velocity_storage = VelocityStorage_init();
    world.paddle_storage = PaddleStorage_init();
    world.ball_storage = BallStorage_init();

    return world;
}

void destroy_world(World *world) {
    PositionStorage_destroy(&world->position_storage);
    VelocityStorage_destroy(&world->velocity_storage);
    PaddleStorage_destroy(&world->paddle_storage);
    BallStorage_destroy(&world->ball_storage);
}
