#pragma once

#include "world.h"
#include <stdint.h>
#include <raylib.h>

#define MAX_SYSTEMS 128

typedef int32_t SystemID;

typedef int (*SystemFn)(World *world);
typedef struct System {
    SystemFn fn;
    SystemID id;
} System;

typedef struct SystemManager {
    System systems[MAX_SYSTEMS];
    uint8_t count;
} SystemManager;

bool init_systems(void);
SystemID system_add(SystemFn fn);
void system_remove(SystemID id);
int16_t run_systems(World *world);

int move_system(World *world);
int debug_system(World *world);
