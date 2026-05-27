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
    PositionStorage storage = {0};
    for (int i = 0; i < ENTITY_MAX_COUNT; i++) {
        storage.sparse_entities[i] = 0;
        storage.dense_data[i] = (Position) { 0 };
        storage.dense_entities[i] = (Entity) { 0 };
    }
    return storage;
}

void PositionStorage_destroy(PositionStorage *storage) {
    (void)storage
}

bool has_position(PositionStorage *storage, Entity entity) {
    uint32_t idx = get_entity_index(entity),
             gen = get_entity_generation(entity);

    uint32_t dense_index = storage->sparse_entities[idx];
    uint32_t dense_gen = dense_index > 0 ? get_entity_generation(storage->dense_entities[dense_index]) : 0;

    return dense_index > 0 && dense_gen == gen;
}

void upsert_position(PositionStorage *storage, Entity entity, Position position) {
    // optimistic component upsert. does not take into consideration if entity is alive.
    // if the position for that entity exists already we just set it.
    if (has_position(storage, entity)) {
        uint32_t dense_idx = storage->sparse_entities[get_entity_index(entity)];
        storage->dense_data[dense_idx] = position;
        return;
    }
    // if the position does not exist for that entity then we:
    // 1. increment the dense count to get the new dense index
    // 2. update the dense entity array with the entity using the new index
    // 3. update the dense data array with the position
    // 4. finally update the sparse entity array with the dense index on the entity index.
    uint32_t idx = get_entity_index(entity);
    uint32_t dense_index = ++storage->dense_count;
    storage->dense_entities[dense_index] = entity;
    storage->dense_data[dense_index] = position;
    storage->sparse_entities[idx] = storage->dense_count;
}

void remove_position(PositionStorage *storage, Entity entity) {
    if (!has_position(storage, entity)) return;

    uint32_t idx = get_entity_index(entity);
    uint32_t last_d_idx = storage->dense_count,
             d_idx = storage->sparse_entities[idx];
    bool patch = d_idx != last_d_idx;

    // clean up sparse_entities
    storage->sparse_entities[idx] = 0;

    if (patch) {
        // move the last item on the dense array to the removed component
        // and point it's sparse entity map to the new index.
        storage->dense_data[d_idx] = storage->dense_data[last_d_idx];
        storage->dense_entities[d_idx] = storage->dense_entities[last_d_idx];
        storage->sparse_entities[get_entity_index(storage->dense_entities[d_idx])] = d_idx;
    }

    // remove the last item on the dense array.
    storage->dense_entities[last_d_idx] = 0;
    storage->dense_data[last_d_idx] = (Position) { 0 };
    storage->dense_count--;
}

// checks if the entity exists (which already handles gen validation) and
// then just returns the dense_data.
Position *get_position_ptr(PositionStorage *storage, Entity entity) {
    if (!has_position(storage, entity)) return NULL;

    uint32_t dense_idx = storage->sparse_entities[get_entity_index(entity)];
    return &storage->dense_data[dense_idx];
}

// -------------------------------------------------------------------------------------------------------
// Other storages might not need that much of a deep implementation. Not like the singleton storages such 
// as ball and paddle though.
// -------------------------------------------------------------------------------------------------------

void VelocityStorage_destroy(VelocityStorage *storage) {
    (void)storage
}

VelocityStorage VelocityStorage_init(void) {
    VelocityStorage storage;
    for (int i = 0; i < ENTITY_MAX_COUNT; i++) {
        storage.sparse_entities[i] = 0;
        storage.dense_data[i] = (Velocity) { 0 };
        storage.dense_entities[i] = (Entity) { 0 };
    }
    return storage;
}

void BrickStorage_destroy(BrickStorage *storage) {
    (void)storage
}

BrickStorage BrickStorage_init(void) {
    BrickStorage storage;
    for (int i = 0; i < ENTITY_MAX_COUNT; i++) {
        storage.sparse_entities[i] = 0;
        storage.dense_data[i] = (Brick) { 0 };
        storage.dense_entities[i] = (Entity) { 0 };
    }
    return storage;
}

PaddleStorage PaddleStorage_init(void) {
    PaddleStorage storage;
    storage.data = (Paddle) { 0 };
    return storage;
}

void PaddleStorage_destroy(PaddleStorage *storage) {
    storage->entity = (Entity) { 0 };
    storage->data = (Paddle) { 0 };
}

BallStorage BallStorage_init(void) {
    BallStorage storage;
    storage.data = (Ball) { 0 };
    return storage;
}

void BallStorage_destroy(BallStorage *storage) {
    storage->entity = (Entity) { 0 };
    storage->data = (Ball) { 0 };
}
