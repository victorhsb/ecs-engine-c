#include "world.h"
#include "entity.h"
#include "storage.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/syslimits.h>


World init_world(void) {
    World world = {0};
    world.position_storage = PositionStorage_init();
    world.velocity_storage = VelocityStorage_init();
    world.paddle_storage = PaddleStorage_init();
    world.ball_storage = BallStorage_init();
    world.brick_storage = BrickStorage_init();

    world.free_count = ENTITY_MAX_COUNT;
    for (uint32_t i = 0; i < ENTITY_MAX_COUNT; i++) {
        world.free_entities[i] = i;
        world.generations[i] = 0;
        world.alive[i] = false;
    }

    return world;
}

void destroy_world(World *world) {
    PositionStorage_destroy(&world->position_storage);
    VelocityStorage_destroy(&world->velocity_storage);
    PaddleStorage_destroy(&world->paddle_storage);
    BallStorage_destroy(&world->ball_storage);
    BrickStorage_destroy(&world->brick_storage);

    free(world->free_entities);
    free(world->generations);
    free(world->alive);
    free(world);
}

// entity_alive will check if the given entity is currently alive and it's
// generation matches the gen tracker
bool entity_alive(World *world, Entity id) {
    uint32_t idx = get_entity_index(id),
             generation = get_entity_generation(id);
    return world->alive[idx] && world->generations[idx] == generation;
}

// create_entity
Entity create_entity(World *world) {
    assert(world->free_count > 0);

    // decrease the available_entities count
    uint32_t index = world->free_entities[--world->free_count];
    world->alive[index] = true;

    return entity_init(index, world->generations[index]);
}

void destroy_entity(World *world, Entity entity) {
    uint32_t idx = get_entity_index(entity);
    if (!world->alive[idx]) return;

    world->alive[idx] = false;
    // both increment free_count && assign the available index to the
    // end of the free_entities array
    world->free_entities[world->free_count++] = idx;
    world->generations[idx]++;
}
