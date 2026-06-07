#include "system.h"
#include "entity.h"
#include "events.h"
#include "input.h"
#include "storage.h"
#include <assert.h>

static int movement_system(World *world) {
    Velocity *vel = get_velocity(&world->velocity_storage, world->player);
    assert(vel != NULL);
    Paddle *pad = get_paddle(&world->paddle_storage, world->player);
    assert(pad != NULL);
    vel->x = world->input_state.horizontal_axis * pad->speed;
    return 1;
}

static int physics_system(World *world) {
    VelocityStorage *vs = &world->velocity_storage;
    for (size_t i = 0; i < vs->dense_count; i++) {
        Velocity vel = vs->dense_data[i];
        if (vel.x == 0 && vel.y == 0)
            continue;
        Entity en = vs->dense_entities[i];
        Position *pos = get_position(&world->position_storage, en);
        assert(pos != NULL);
        pos->x += vel.x * world->game_state.delta_time;
        pos->y += vel.y * world->game_state.delta_time;
    }
    return 1;
}

static int events_system(World *world) {
    bool *paused = &world->game_state.paused;
    // pressing esc once = pauses
    // pressing esc twice = exits
    if (world->input_state.events[EVENT_ESC]) {
        if (*paused)
            EventBus_add_exit(&world->event_bus, (void *)true);
        *paused = true;
    }
    if (world->input_state.events[EVENT_ACTION]) {
        if (*paused) {
            *paused = false;
        }
    }
    return 1;
}

static SystemFn systems[] = {
    movement_system,
    physics_system,
    events_system,
};

void run_systems(World *world) {
    if (!world)
        return;
    size_t count = sizeof(systems) / sizeof(systems[0]);
    for (size_t i = 0; i < count; i++) {
        if (systems[i](world) < 0)
            return;
    }
}
