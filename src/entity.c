#include "entity.h"

uint32_t get_entity_generation(Entity entity) {
    return (uint32_t)(entity >> 32);
}

uint32_t get_entity_index(Entity entity) {
    return (uint32_t)(entity & 0xFFFFFFFF);
}

int is_entity_valid(Entity entity) {
    return get_entity_generation(entity) != 0 && get_entity_index(entity) != 0;
}

Entity create_entity(uint32_t index, uint32_t generation) {
    return (Entity)((uint64_t)generation << 32 | index);
}
