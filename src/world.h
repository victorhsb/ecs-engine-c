#pragma once

#include "storage.h"
#include "entity.h"
#include <stdint.h>

#define ENTITY_MAX_COUNT 1024

typedef struct GameState {
    uint32_t score;
    bool paused;
    bool game_over;
} GameState;

typedef struct World {
    // Tracks whether an entity is alive or not. This is a sparse
    // array where each index corresponds to an entity ID.
    bool alive[ENTITY_MAX_COUNT];
    // List of available entities. This is a dense array where
    // each element corresponds to an entity ID that has been freed for use.
    uint32_t free_entities[ENTITY_MAX_COUNT];
    // logical tracker to the number of free entities.
    // accounts for not having a len(free_entities) :)
    uint32_t free_count;
    // tracker of generations. if an Entity handle does not match
    // it's generation then something went wrong perhaps?
    uint32_t generations[ENTITY_MAX_COUNT];

    // keeps track of the amount of initialized entities.
    uint32_t entity_count;

    // singleton most important player
    Entity player;

    // now the actual components sparse storages
    PositionStorage position_storage;
    VelocityStorage velocity_storage;
    PaddleStorage paddle_storage;
    BallStorage ball_storage;
    BrickStorage brick_storage;

    // game state
    GameState game_state;
} World;

World init_world(void);
void destroy_world(World *world);
