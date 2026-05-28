#pragma once

#include "component.h"
#include "entity.h"
#include <stdint.h>

#define ENTITY_MAX_COUNT 1024

// reference implementation to the macro wizardry below
typedef struct PositionStorage {
    // sparse entities is the sparse array of indexes that maps
    // entity index with storage index.
    // The actual dense index is i+1 so we can consider 0 as absence.
    uint32_t sparse_entities[ENTITY_MAX_COUNT];

    // dense count and dense_entities is the dense map of entities.
    // dense_count points to the end of the array while dense_entities
    // stores the entity at each dense index.
    uint32_t dense_count;
    Entity dense_entities[ENTITY_MAX_COUNT + 1]; // +1 for unused 0
    // at last the dense data stores the actual position at the same
    // dense index as the dense_entity
    Position dense_data[ENTITY_MAX_COUNT + 1]; // +1 for unused 0
} PositionStorage;

PositionStorage PositionStorage_init(void);
void PositionStorage_destroy(PositionStorage *storage);
bool has_position(PositionStorage const *storage, Entity const entity);
void upsert_position(PositionStorage *storage, Entity entity, Position position);
void remove_position(PositionStorage *storage, Entity entity);
Position *get_position(PositionStorage *storage, Entity const entity);

// C macro wizardry to dynamically generate structs
#define GENERATE_COMPONENT_STORAGE(base, target) \
    typedef struct target { \
        /* macro wizardry bullshit. see PositionStorage for reference */ \
        uint32_t sparse_entities[ENTITY_MAX_COUNT]; \
        uint32_t dense_count; \
        base dense_data[ENTITY_MAX_COUNT+1]; \
        Entity dense_entities[ENTITY_MAX_COUNT +1]; \
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

GENERATE_COMPONENT_STORAGE(Velocity, VelocityStorage)
GENERATE_COMPONENT_STORAGE(Brick, BrickStorage)
GENERATE_COMPONENT_SINGLE_STORAGE(Paddle, PaddleStorage)
// ball starts as a singleton but i might just make it into a
// storage in the future if i implement multi-ball support.
GENERATE_COMPONENT_SINGLE_STORAGE(Ball, BallStorage)
