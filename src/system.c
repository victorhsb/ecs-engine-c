#include "system.h"
#include "entity.h"
#include "events.h"
#include "input.h"
#include "storage.h"
#include <assert.h>
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

int move_system(World *world) {
    if (world->input_state.horizontal_axis != 0) {
        Paddle *paddle = get_paddle(&world->paddle_storage, world->player);
        Position *pos = get_position(&world->position_storage, world->player);
        if (pos) {
            pos->x += world->input_state.horizontal_axis *
                      world->game_state.delta_time * (float)paddle->speed;
        }
        return 1;
    }
    return 0;
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
