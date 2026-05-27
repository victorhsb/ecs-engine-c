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
    EntityManager mgr = {0};
    mgr.free_count = ENTITY_MAX_COUNT;
    for (uint32_t i = 0; i < ENTITY_MAX_COUNT; i++) {
        mgr.free_entities[i] = i;
        mgr.generations[i] = 0;
        mgr.alive[i] = false;
    }
    return mgr;
}

void destroy_entity_manager(EntityManager *mgr) {
    (void)mgr;
}

bool entity_alive(EntityManager *mgr, Entity id) {
    uint32_t idx = get_entity_index(id),
             generation = get_entity_generation(id);
    return mgr->alive[idx] && mgr->generations[idx] == generation;
}

Entity create_entity(EntityManager *mgr) {
    assert(mgr->free_count > 0);

    uint32_t index = mgr->free_entities[--mgr->free_count];
    mgr->alive[index] = true;

    return entity_init(index, mgr->generations[index]);
}

void destroy_entity(EntityManager *mgr, Entity entity) {
    uint32_t idx = get_entity_index(entity);
    if (!mgr->alive[idx]) return;

    mgr->alive[idx] = false;
    mgr->free_entities[mgr->free_count++] = idx;
    mgr->generations[idx]++;
}
