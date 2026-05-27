#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef uint64_t Entity;

#define ENTITY_MAX_COUNT 1024

typedef struct EntityManager {
    bool alive[ENTITY_MAX_COUNT];
    uint32_t free_entities[ENTITY_MAX_COUNT];
    uint32_t free_count;
    uint32_t generations[ENTITY_MAX_COUNT];
    uint32_t entity_count;
} EntityManager;

uint32_t get_entity_generation(Entity entity);
uint32_t get_entity_index(Entity entity);
int is_entity_valid(Entity entity);
Entity entity_init(uint32_t index, uint32_t generation);

EntityManager init_entity_manager(void);
void destroy_entity_manager(EntityManager *mgr);
bool entity_alive(EntityManager *mgr, Entity id);
Entity create_entity(EntityManager *mgr);
void destroy_entity(EntityManager *mgr, Entity entity);
