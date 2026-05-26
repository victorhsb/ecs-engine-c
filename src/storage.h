#pragma once

#include "component.h"
#include "entity.h"

#define ENTITY_MAX_COUNT 1024

// C macro wizardry to dynamically generate structs
#define GENERATE_COMPONENT_STORAGE(base, target) \
    typedef struct target { \
        uint32_t sparse_entities[ENTITY_MAX_COUNT]; \
        base dense_data[ENTITY_MAX_COUNT]; \
        Entity dense_entities[ENTITY_MAX_COUNT]; \
    } target; \
    void target##_destroy(target *storage); \
    target target##_init(void);

// has the same interface of the standard storage but stores a singleton.
#define GENERATE_COMPONENT_SINGLE_STORAGE(base, target) \
    typedef struct target { \
        Entity entity; \
        base data; \
    } target; \
    target target##_init(void); \
    void target##_destroy(target *storage);

GENERATE_COMPONENT_STORAGE(Position, PositionStorage)
GENERATE_COMPONENT_STORAGE(Velocity, VelocityStorage)
GENERATE_COMPONENT_SINGLE_STORAGE(Paddle, PaddleStorage)
// ball starts as a singleton but i might just make it into a
// storage in the future if i implement multi-ball support.
GENERATE_COMPONENT_SINGLE_STORAGE(Ball, BallStorage)
