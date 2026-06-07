#pragma once

#include "world.h"
#include <raylib.h>
#include <stdint.h>

typedef int (*SystemFn)(World *world);

void run_systems(World *world);
