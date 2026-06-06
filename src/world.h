#pragma once

#include "events.h"
#include "storage.h"
#include "entity.h"
#include <stdint.h>
#include "input.h"

typedef struct GameState {
    float delta_time;
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

    InputState input_state;
    GameState game_state;
    EventBus event_bus;
} World;

GameState GameState_init(void);

World init_world(void);
void destroy_world(World *world);
