#pragma once

#include "storage.h"
#include "entity.h"
#include <stdint.h>

typedef struct GameState {
    uint32_t score;
    bool paused;
    bool game_over;
} GameState;

typedef struct World {
    EntityManager entity_manager;

    Entity player;

    PositionStorage position_storage;
    VelocityStorage velocity_storage;
    PaddleStorage paddle_storage;
    BallStorage ball_storage;
    BrickStorage brick_storage;

    GameState game_state;
} World;

World init_world(void);
void destroy_world(World *world);
