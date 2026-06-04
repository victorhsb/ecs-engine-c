#include "system.h"

static SystemManager *manager = NULL;

static const SystemID NULL_SYS = -1;

bool init_systems(void) {
    if (manager != NULL) return true;
    manager = calloc(1, sizeof(SystemManager));
    return manager != NULL;
}

void deinit_systems(void) {
    free(manager);
    manager = NULL;
}

int16_t run_systems(World *world) {
    if (!manager || !world) return -1;
    for (uint8_t i = 0; i < manager->count; i++) {
        if (manager->systems[i].fn(world) < 0) return -1;
    }
    return (int16_t)manager->count;
}

SystemID system_add(SystemFn fn) {
    if (!manager || !fn || manager->count >= MAX_SYSTEMS) return NULL_SYS;
    SystemID id = (SystemID)manager->count + 1;
    manager->systems[manager->count] = (System){ .fn = fn, .id = id };
    manager->count++;
    return id;
}

void system_remove(SystemID id) {
    if (!manager || id == NULL_SYS) return;
    for (uint8_t i = 0; i < manager->count; i++) {
        if (manager->systems[i].id == id) {
            manager->systems[i] = manager->systems[manager->count - 1];
            manager->systems[manager->count - 1] = (System){0};
            manager->count--;
            return;
        }
    }
}
