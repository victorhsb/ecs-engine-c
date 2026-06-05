#include "entity.h"
#include <assert.h>

uint32_t get_entity_generation(Entity entity) {
    return (uint32_t)(entity >> 32);
}

uint32_t get_entity_index(Entity entity) {
    return (uint32_t)(entity & 0xFFFFFFFF);
}

int is_entity_valid(Entity entity) {
    return get_entity_generation(entity) != 0 && get_entity_index(entity) != 0;
}

Entity entity_init(uint32_t index, uint32_t generation) {
    return (Entity)((uint64_t)generation << 32 | index);
}

EntityManager init_entity_manager(void) {
    return (EntityManager){0};
}

void destroy_entity_manager(EntityManager *mgr) {
    (void)mgr;
}

bool entity_alive(EntityManager *mgr, Entity id) {
    uint32_t idx = get_entity_index(id),
             generation = get_entity_generation(id);
    return mgr->sparse_alive[idx] && mgr->sparse_generations[idx] == generation;
}

// create entity is responsible for returning the next available entity for use
// if there were any entities freed then it should reclaim it and bump it's generation.
// if there are no freed entities then it should pick the next gen with zeroed generation 0.
Entity create_entity(EntityManager *mgr) {
    if (mgr->dense_free_entities_count > 0) {
        uint32_t index = mgr->dense_free_entities[--mgr->dense_free_entities_count];
        mgr->sparse_alive[index] = true;
        return entity_init(index, ++mgr->sparse_generations[index]);
    }

    assert(mgr->sparse_generations_count < ENTITY_MAX_COUNT);
    uint32_t idx = mgr->sparse_generations_count++;
    Entity entt = entity_init(idx, ++mgr->sparse_generations[idx]);
    return entt;
}

void destroy_entity(EntityManager *mgr, Entity entity) {
    uint32_t idx = get_entity_index(entity);
    if (!mgr->sparse_alive[idx]) return;

    mgr->sparse_alive[idx] = false;
    mgr->dense_free_entities[mgr->dense_free_entities_count++] = idx;
    mgr->sparse_generations[idx]++;
}
