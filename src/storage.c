#include "storage.h"
#include "component.h"
#include "entity.h"
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
//
// Position Storage serves as the reference point for all the other storage implementations
// that may or may not be written using macros just for the sake of practicity.
//
// What are the functions i need to work on Positions?
// create, fetch, check, delete
// oh... it's a CRUD?
//

PositionStorage PositionStorage_init(void) {
    return (PositionStorage){0};
}

void PositionStorage_destroy(PositionStorage *storage) {
    (void)storage;
}

bool has_position(PositionStorage const *storage, Entity const entity) {
    uint32_t idx = get_entity_index(entity),
             gen = get_entity_generation(entity),
             dense_index = storage->sparse_entities[idx],
             dense_gen = dense_index > 0 ? get_entity_generation(storage->dense_entities[dense_index]) : 0;
    return dense_index > 0 && dense_gen == gen;
}

void upsert_position(PositionStorage *storage, Entity entity, Position position) {
    // optimistic component upsert. does not take into consideration if entity is alive.
    // if the position for that entity exists already we just set it.
    uint32_t entity_idx = get_entity_index(entity),
             dense_idx = storage->sparse_entities[entity_idx];
    if (dense_idx > 0) {
        uint32_t stored_gen = get_entity_generation(storage->dense_entities[dense_idx]),
                 incoming_gen = get_entity_generation(entity);
        if (stored_gen == incoming_gen) {
            storage->dense_data[dense_idx] = position;
            return;
        }

        // stale entity (gen mismatch) = overwrite component
        storage->dense_entities[dense_idx] = entity;
        storage->dense_data[dense_idx] = position;
        return;
    }

    // if the position does not exist for that entity then we:
    // 1. increment the dense count to get the new dense index
    // 2. update the dense entity array with the entity using the new index
    // 3. update the dense data array with the position
    // 4. finally update the sparse entity array with the dense index on the entity index.
    assert(storage->dense_count < ENTITY_MAX_COUNT);
    uint32_t new_dense_index = ++storage->dense_count;
    storage->dense_entities[new_dense_index] = entity;
    storage->dense_data[new_dense_index] = position;
    storage->sparse_entities[entity_idx] = storage->dense_count;
}

void remove_position(PositionStorage *storage, Entity entity) {
    if (!has_position(storage, entity)) return;

    uint32_t entity_index = get_entity_index(entity),
             last_index = storage->dense_count,
             dense_index = storage->sparse_entities[entity_index];

    // clean up sparse_entities
    storage->sparse_entities[entity_index] = 0;

    if (dense_index != last_index) {
        // move the last item on the dense array to the removed component
        // and point it's sparse entity map to the new index.
        storage->dense_data[dense_index] = storage->dense_data[last_index];
        storage->dense_entities[dense_index] = storage->dense_entities[last_index];
        storage->sparse_entities[get_entity_index(storage->dense_entities[dense_index])] = dense_index;
    }

    // remove the last item on the dense array.
    storage->dense_entities[last_index] = 0;
    storage->dense_data[last_index] = (Position) { 0 };
    storage->dense_count--;
}

// checks if the entity exists (which already handles gen validation) and
// then just returns the dense_data.
Position *get_position(PositionStorage *storage, Entity const entity) {
    if (!has_position(storage, entity)) return NULL;

    uint32_t dense_idx = storage->sparse_entities[get_entity_index(entity)];
    return &storage->dense_data[dense_idx];
}

// -------------------------------------------------------------------------------------------------------
// Other storages might not need that much of a deep implementation. Not like the singleton storages such
// as ball and paddle though.
// -------------------------------------------------------------------------------------------------------

#define GENERATE_STORAGE_INIT(tgt) \
void tgt##_destroy(tgt *storage) { \
    (void)storage; \
} \
tgt tgt##_init(void) { \
    return (tgt){0}; \
}

#define GENERATE_STORAGE_CRUD(COMP, PREFIX) \
bool has_##PREFIX(COMP##Storage *storage, Entity entity) { \
    uint32_t idx = get_entity_index(entity); \
    uint32_t gen = get_entity_generation(entity); \
    uint32_t dense_index = storage->sparse_entities[idx]; \
    uint32_t dense_gen = dense_index > 0 ? get_entity_generation(storage->dense_entities[dense_index]) : 0; \
    return dense_index > 0 && dense_gen == gen; \
} \
\
void upsert_##PREFIX(COMP##Storage *storage, Entity entity, COMP value) { \
    uint32_t entity_idx = get_entity_index(entity); \
    uint32_t dense_idx = storage->sparse_entities[entity_idx]; \
    if (dense_idx > 0) { \
        uint32_t stored_gen = get_entity_generation(storage->dense_entities[dense_idx]), \
                 incoming_gen = get_entity_generation(entity); \
        if (stored_gen == incoming_gen) { \
            storage->dense_data[dense_idx] = value; \
            return; \
        } \
        storage->dense_entities[dense_idx] = entity; \
        storage->dense_data[dense_idx] = value; \
        return; \
    } \
    assert(storage->dense_count < ENTITY_MAX_COUNT); \
    uint32_t new_dense_index = ++storage->dense_count; \
    storage->dense_entities[new_dense_index] = entity; \
    storage->dense_data[new_dense_index] = value; \
    storage->sparse_entities[entity_idx] = storage->dense_count; \
} \
\
void remove_##PREFIX(COMP##Storage *storage, Entity entity) { \
    if (!has_##PREFIX(storage, entity)) return; \
    uint32_t entity_index = get_entity_index(entity); \
    uint32_t last_index = storage->dense_count; \
    uint32_t dense_index = storage->sparse_entities[entity_index]; \
    storage->sparse_entities[entity_index] = 0; \
    if (dense_index != last_index) { \
        storage->dense_data[dense_index] = storage->dense_data[last_index]; \
        storage->dense_entities[dense_index] = storage->dense_entities[last_index]; \
        storage->sparse_entities[get_entity_index(storage->dense_entities[dense_index])] = dense_index; \
    } \
    storage->dense_entities[last_index] = 0; \
    storage->dense_data[last_index] = (COMP) { 0 }; \
    storage->dense_count--; \
} \
\
COMP *get_##PREFIX(COMP##Storage *storage, Entity entity) { \
    if (!has_##PREFIX(storage, entity)) return NULL; \
    uint32_t dense_idx = storage->sparse_entities[get_entity_index(entity)]; \
    return &storage->dense_data[dense_idx]; \
}

GENERATE_STORAGE_INIT(VelocityStorage)
GENERATE_STORAGE_CRUD(Velocity, velocity)
GENERATE_STORAGE_INIT(BrickStorage)
GENERATE_STORAGE_CRUD(Brick, brick)

// singletons require a different impl

PaddleStorage PaddleStorage_init(void) {
  PaddleStorage storage = {0};
  return storage;
}

BallStorage BallStorage_init(void) {
  BallStorage storage = {0};
  return storage;
}

void upsert_paddle(PaddleStorage *storage, Entity entity, Paddle paddle) {
    storage->entity = entity;
    storage->data = paddle;
}

Paddle *get_paddle(PaddleStorage *storage, Entity entity) {
    if (storage->entity == entity) return &storage->data;
    return NULL;
}

void upsert_ball(BallStorage *storage, Entity entity, Ball ball) {
    storage->entity = entity;
    storage->data = ball;
}

Ball *get_ball(BallStorage *storage, Entity entity) {
    if (storage->entity == entity) return &storage->data;
    return NULL;
}
