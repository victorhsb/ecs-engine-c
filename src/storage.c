#include "storage.h"
#include <stdlib.h>

#define GENERATE_COMPONENT_STORAGE_API(base, target) \
    void target##_destroy(target *storage) { \
        free(storage->dense_entities); \
        free(storage->sparse_entities); \
        free(storage->dense_data); \
    } \
    target target##_init(void) { \
        target storage; \
        for (int i = 0; i < ENTITY_MAX_COUNT; i++) { \
            storage.sparse_entities[i] = 0; \
            storage.dense_data[i] = (base) { 0 }; \
            storage.dense_entities[i] = (Entity) { 0 }; \
        } \
        return storage; \
    }
#define GENERATE_COMPONENT_SINGLE_STORAGE_API(base, target) \
    target target##_init(void) { \
        target storage; \
        storage.data = (base) { 0 }; \
        return storage; \
    } \
    void target##_destroy(target *storage) { \
        storage->entity = (Entity) { 0 }; \
        storage->data = (base) { 0 }; \
    }

GENERATE_COMPONENT_STORAGE_API(Position, PositionStorage)
GENERATE_COMPONENT_STORAGE_API(Velocity, VelocityStorage)
GENERATE_COMPONENT_SINGLE_STORAGE_API(Paddle, PaddleStorage)
// ball starts as a singleton but i might just make it into a
// storage in the future if i implement multi-ball support.
GENERATE_COMPONENT_SINGLE_STORAGE_API(Ball, BallStorage)
