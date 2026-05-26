#pragma once

#include "storage.h"
#include "entity.h"

#define ENTITY_MAX_COUNT 1024

typedef struct World {
    // Tracks whether an entity is alive or not
    bool alive[ENTITY_MAX_COUNT];
    // List of available entities
    uint32_t available_entities[ENTITY_MAX_COUNT];
    // tracker of generations. if an Entity handle does not match
    // it's generation then something went wrong perhaps?
    uint32_t generations[ENTITY_MAX_COUNT];

    Entity player;

    // now the actual components sparse storages
    PositionStorage position_storage;
    VelocityStorage velocity_storage;
    PaddleStorage paddle_storage;
    BallStorage ball_storage;
} World;

World init_world(void);
void destroy_world(World *world);
