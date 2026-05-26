#pragma once

#include <stdint.h>

typedef uint64_t Entity;

uint32_t get_entity_generation(Entity entity);
uint32_t get_entity_index(Entity entity);
int is_entity_valid(Entity entity);
Entity create_entity(uint32_t index, uint32_t generation);
