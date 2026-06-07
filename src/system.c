#include "system.h"
#include "entity.h"
#include "events.h"
#include "input.h"
#include "storage.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static SystemManager *manager = NULL;

static const SystemID NULL_SYS = -1;

bool init_systems(void) {
    if (manager != NULL)
        return true;
    manager = calloc(1, sizeof(SystemManager));
    return manager != NULL;
}

void deinit_systems(void) {
    free(manager);
    manager = NULL;
}

int16_t run_systems(World *world) {
    if (!manager || !world)
        return -1;
    for (uint8_t i = 0; i < manager->count; i++) {
        if (manager->systems[i].fn(world) < 0)
            return -1;
    }
    return (int16_t)manager->count;
}

SystemID system_add(SystemFn fn) {
    if (!manager || !fn || manager->count >= MAX_SYSTEMS)
        return NULL_SYS;
    SystemID id = (SystemID)manager->count + 1;
    manager->systems[manager->count] = (System){.fn = fn, .id = id};
    manager->count++;
    return id;
}

void system_remove(SystemID id) {
    if (!manager || id == NULL_SYS)
        return;
    for (uint8_t i = 0; i < manager->count; i++) {
        if (manager->systems[i].id == id) {
            manager->systems[i] = manager->systems[manager->count - 1];
            manager->systems[manager->count - 1] = (System){0};
            manager->count--;
            return;
        }
    }
}

int movement_system(World *world) {
    Velocity *vel = get_velocity(&world->velocity_storage, world->player);
    assert(vel != NULL);
    Paddle *pad = get_paddle(&world->paddle_storage, world->player);
    assert(pad != NULL);
    vel->x = world->input_state.horizontal_axis * pad->speed;
    return 1;
}

int physics_system(World *world) {
    VelocityStorage *vs = &world->velocity_storage;
    for (size_t i = 0; i < vs->dense_count; i++) {
        Velocity vel = vs->dense_data[i];
        if (vel.x == 0 && vel.y == 0) continue;
        Entity en = vs->dense_entities[i];
        Position *pos = get_position(&world->position_storage, en);
        assert(pos != NULL);
        pos->x += vel.x * world->game_state.delta_time;
        pos->y += vel.y * world->game_state.delta_time;
    }
    return 1;
}

int debug_system(World *world) {
    if (world->input_state.events[EVENT_ACTION]) {
        // kill the last brick; just for fun
        Entity b = world->brick_storage
                       .dense_entities[world->brick_storage.dense_count];
        remove_brick(&world->brick_storage, b);
        remove_position(&world->position_storage, b);
        destroy_entity(&world->entity_manager, b);
    }
    return 1;
}

int input_system(World *world) {
    if (world->input_state.events[EVENT_ACTION]) {
        if (world->game_state.paused) {
            world->game_state.paused = false;
        }
    }
    if (world->input_state.events[EVENT_ESC]) {
        if (world->game_state.paused)
            EventBus_add_exit(&world->event_bus, (void *)true);
        world->game_state.paused = true;
    }
    return 1;
}
