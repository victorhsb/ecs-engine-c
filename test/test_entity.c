#include "unity.h"
#include "entity.h"

void test_entity_init_packs_generation_and_index(void) {
    Entity e = entity_init(42, 7);
    TEST_ASSERT_EQUAL_UINT32(7, get_entity_generation(e));
    TEST_ASSERT_EQUAL_UINT32(42, get_entity_index(e));
}

void test_entity_init_with_zero_values(void) {
    Entity e = entity_init(0, 0);
    TEST_ASSERT_EQUAL_UINT32(0, get_entity_generation(e));
    TEST_ASSERT_EQUAL_UINT32(0, get_entity_index(e));
}

void test_is_entity_valid_returns_false_for_zero(void) {
    Entity e = entity_init(0, 0);
    TEST_ASSERT_FALSE(is_entity_valid(e));
}

void test_is_entity_valid_returns_true_for_nonzero(void) {
    Entity e = entity_init(1, 1);
    TEST_ASSERT_TRUE(is_entity_valid(e));
}

void test_init_entity_manager_sets_all_dead(void) {
    EntityManager mgr = init_entity_manager();
    for (uint32_t i = 0; i < ENTITY_MAX_COUNT; i++) {
        TEST_ASSERT_FALSE(mgr.alive[i]);
    }
}

void test_init_entity_manager_sets_zero_generations(void) {
    EntityManager mgr = init_entity_manager();
    for (uint32_t i = 0; i < ENTITY_MAX_COUNT; i++) {
        TEST_ASSERT_EQUAL_UINT32(0, mgr.generations[i]);
    }
}

void test_init_entity_manager_free_count_equals_max(void) {
    EntityManager mgr = init_entity_manager();
    TEST_ASSERT_EQUAL_UINT32(ENTITY_MAX_COUNT, mgr.free_count);
}

void test_create_entity_marks_alive(void) {
    EntityManager mgr = init_entity_manager();
    Entity e = create_entity(&mgr);
    uint32_t idx = get_entity_index(e);
    TEST_ASSERT_TRUE(mgr.alive[idx]);
}

void test_create_entity_decrements_free_count(void) {
    EntityManager mgr = init_entity_manager();
    create_entity(&mgr);
    TEST_ASSERT_EQUAL_UINT32(ENTITY_MAX_COUNT - 1, mgr.free_count);
}

void test_create_entity_has_generation_zero(void) {
    EntityManager mgr = init_entity_manager();
    Entity e = create_entity(&mgr);
    TEST_ASSERT_EQUAL_UINT32(0, get_entity_generation(e));
}

void test_entity_alive_returns_true_for_live_entity(void) {
    EntityManager mgr = init_entity_manager();
    Entity e = create_entity(&mgr);
    TEST_ASSERT_TRUE(entity_alive(&mgr, e));
}

void test_destroy_entity_marks_dead(void) {
    EntityManager mgr = init_entity_manager();
    Entity e = create_entity(&mgr);
    uint32_t idx = get_entity_index(e);
    destroy_entity(&mgr, e);
    TEST_ASSERT_FALSE(mgr.alive[idx]);
}

void test_destroy_entity_increments_generation(void) {
    EntityManager mgr = init_entity_manager();
    Entity e = create_entity(&mgr);
    uint32_t idx = get_entity_index(e);
    destroy_entity(&mgr, e);
    TEST_ASSERT_EQUAL_UINT32(1, mgr.generations[idx]);
}

void test_entity_alive_returns_false_after_destroy(void) {
    EntityManager mgr = init_entity_manager();
    Entity e = create_entity(&mgr);
    destroy_entity(&mgr, e);
    TEST_ASSERT_FALSE(entity_alive(&mgr, e));
}

void test_destroy_entity_increments_free_count(void) {
    EntityManager mgr = init_entity_manager();
    Entity e = create_entity(&mgr);
    destroy_entity(&mgr, e);
    TEST_ASSERT_EQUAL_UINT32(ENTITY_MAX_COUNT, mgr.free_count);
}

void test_reused_entity_gets_new_generation(void) {
    EntityManager mgr = init_entity_manager();
    Entity e1 = create_entity(&mgr);
    uint32_t idx = get_entity_index(e1);
    destroy_entity(&mgr, e1);
    Entity e2 = create_entity(&mgr);
    TEST_ASSERT_EQUAL_UINT32(idx, get_entity_index(e2));
    TEST_ASSERT_EQUAL_UINT32(1, get_entity_generation(e2));
    TEST_ASSERT_TRUE(entity_alive(&mgr, e2));
}

void test_old_entity_handle_not_alive_after_reuse(void) {
    EntityManager mgr = init_entity_manager();
    Entity e1 = create_entity(&mgr);
    destroy_entity(&mgr, e1);
    Entity e2 = create_entity(&mgr);
    (void)e2;
    TEST_ASSERT_FALSE(entity_alive(&mgr, e1));
}

void test_destroy_already_dead_entity_is_noop(void) {
    EntityManager mgr = init_entity_manager();
    Entity e = create_entity(&mgr);
    destroy_entity(&mgr, e);
    uint32_t free_before = mgr.free_count;
    destroy_entity(&mgr, e);
    TEST_ASSERT_EQUAL_UINT32(free_before, mgr.free_count);
}
