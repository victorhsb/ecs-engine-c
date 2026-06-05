#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef uint64_t Entity;

#define ENTITY_MAX_COUNT 5000

typedef struct EntityManager {
    // Tracks whether an entity is alive or not.
    // This is a sparse array where each index corresponds to an entity ID.
    bool sparse_alive[ENTITY_MAX_COUNT];

    // List of available entities. This is a dense array where
    // each element corresponds to an entity ID that has been freed for use.
    uint32_t dense_free_entities[ENTITY_MAX_COUNT];

    // logical tracker to the number of free entities.
    // accounts for not having a len(free_entities) :)
    uint32_t dense_free_entities_count;

    // Tracks the generation of each entity. This is used to detect
    // when an entity is reused (i.e., when its index is reused after being freed).
    // This is a sparse array where each index corresponds to an entity ID.
    uint32_t sparse_generations[ENTITY_MAX_COUNT];

    // tracks the amount of generations currently in use.
    uint32_t sparse_generations_count;
} EntityManager;

//
// entity utils
//
uint32_t get_entity_generation(Entity entity);
uint32_t get_entity_index(Entity entity);
int is_entity_valid(Entity entity);
Entity entity_init(uint32_t index, uint32_t generation);

//
// entity management
//
EntityManager init_entity_manager(void);
void destroy_entity_manager(EntityManager *mgr);
bool entity_alive(EntityManager *mgr, Entity id);
Entity create_entity(EntityManager *mgr);
void destroy_entity(EntityManager *mgr, Entity entity);
