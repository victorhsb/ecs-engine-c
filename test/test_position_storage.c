#include "unity.h"
#include "storage.h"

void test_init_dense_count_is_zero(void) {
    PositionStorage s = PositionStorage_init();
    TEST_ASSERT_EQUAL_UINT32(0, s.dense_count);
}

void test_init_sparse_all_zero(void) {
    PositionStorage s = PositionStorage_init();
    for (int i = 0; i < ENTITY_MAX_COUNT; i++) {
        TEST_ASSERT_EQUAL_UINT32(0, s.sparse_entities[i]);
    }
}

void test_init_dense_data_all_zero(void) {
    PositionStorage s = PositionStorage_init();
    for (int i = 0; i < ENTITY_MAX_COUNT; i++) {
        TEST_ASSERT_EQUAL_INT(0, s.dense_data[i].x);
        TEST_ASSERT_EQUAL_INT(0, s.dense_data[i].y);
    }
}

void test_init_dense_entities_all_zero(void) {
    PositionStorage s = PositionStorage_init();
    for (int i = 0; i < ENTITY_MAX_COUNT; i++) {
        TEST_ASSERT_EQUAL_UINT64(0, s.dense_entities[i]);
    }
}

void test_has_position_false_after_init(void) {
    PositionStorage s = PositionStorage_init();
    TEST_ASSERT_FALSE(has_position(&s, entity_init(1, 1)));
}

void test_upsert_increments_dense_count(void) {
    PositionStorage s = PositionStorage_init();
    upsert_position(&s, entity_init(1, 1), (Position){5, 10});
    TEST_ASSERT_EQUAL_UINT32(1, s.dense_count);
}

void test_upsert_stores_entity_in_dense(void) {
    PositionStorage s = PositionStorage_init();
    Entity e = entity_init(1, 1);
    upsert_position(&s, e, (Position){5, 10});
    uint32_t dense_idx = s.sparse_entities[1];
    TEST_ASSERT_EQUAL_UINT64(e, s.dense_entities[dense_idx]);
}

void test_upsert_stores_data_in_dense(void) {
    PositionStorage s = PositionStorage_init();
    Entity e = entity_init(1, 1);
    upsert_position(&s, e, (Position){42, 99});
    uint32_t dense_idx = s.sparse_entities[1];
    TEST_ASSERT_EQUAL_INT(42, s.dense_data[dense_idx].x);
    TEST_ASSERT_EQUAL_INT(99, s.dense_data[dense_idx].y);
}

void test_upsert_updates_sparse(void) {
    PositionStorage s = PositionStorage_init();
    upsert_position(&s, entity_init(1, 1), (Position){5, 10});
    TEST_ASSERT_EQUAL_UINT32(1, s.sparse_entities[1]);
}

void test_upsert_multiple_entities(void) {
    PositionStorage s = PositionStorage_init();
    upsert_position(&s, entity_init(1, 1), (Position){1, 1});
    upsert_position(&s, entity_init(2, 1), (Position){2, 2});
    upsert_position(&s, entity_init(3, 1), (Position){3, 3});
    TEST_ASSERT_EQUAL_UINT32(3, s.dense_count);
    TEST_ASSERT_TRUE(s.sparse_entities[1] > 0);
    TEST_ASSERT_TRUE(s.sparse_entities[2] > 0);
    TEST_ASSERT_TRUE(s.sparse_entities[3] > 0);
}

void test_has_position_true_after_upsert(void) {
    PositionStorage s = PositionStorage_init();
    Entity e = entity_init(1, 1);
    upsert_position(&s, e, (Position){5, 10});
    TEST_ASSERT_TRUE(has_position(&s, e));
}

void test_has_position_false_for_wrong_generation(void) {
    PositionStorage s = PositionStorage_init();
    upsert_position(&s, entity_init(1, 1), (Position){5, 10});
    TEST_ASSERT_FALSE(has_position(&s, entity_init(1, 2)));
}

void test_has_position_false_for_different_index(void) {
    PositionStorage s = PositionStorage_init();
    upsert_position(&s, entity_init(1, 1), (Position){5, 10});
    TEST_ASSERT_FALSE(has_position(&s, entity_init(2, 1)));
}

void test_upsert_update_does_not_increment_dense_count(void) {
    PositionStorage s = PositionStorage_init();
    Entity e = entity_init(1, 1);
    upsert_position(&s, e, (Position){5, 10});
    upsert_position(&s, e, (Position){99, 88});
    TEST_ASSERT_EQUAL_UINT32(1, s.dense_count);
}

void test_upsert_update_changes_data(void) {
    PositionStorage s = PositionStorage_init();
    Entity e = entity_init(1, 1);
    upsert_position(&s, e, (Position){5, 10});
    upsert_position(&s, e, (Position){99, 88});
    uint32_t dense_idx = s.sparse_entities[1];
    TEST_ASSERT_EQUAL_INT(99, s.dense_data[dense_idx].x);
    TEST_ASSERT_EQUAL_INT(88, s.dense_data[dense_idx].y);
}

void test_remove_makes_has_position_false(void) {
    PositionStorage s = PositionStorage_init();
    Entity e = entity_init(1, 1);
    upsert_position(&s, e, (Position){5, 10});
    remove_position(&s, e);
    TEST_ASSERT_FALSE(has_position(&s, e));
}

void test_remove_decrements_dense_count(void) {
    PositionStorage s = PositionStorage_init();
    Entity e = entity_init(1, 1);
    upsert_position(&s, e, (Position){5, 10});
    remove_position(&s, e);
    TEST_ASSERT_EQUAL_UINT32(0, s.dense_count);
}

void test_remove_clears_sparse(void) {
    PositionStorage s = PositionStorage_init();
    upsert_position(&s, entity_init(1, 1), (Position){5, 10});
    remove_position(&s, entity_init(1, 1));
    TEST_ASSERT_EQUAL_UINT32(0, s.sparse_entities[1]);
}

void test_remove_nonexistent_is_noop(void) {
    PositionStorage s = PositionStorage_init();
    remove_position(&s, entity_init(99, 1));
    TEST_ASSERT_EQUAL_UINT32(0, s.dense_count);
}

void test_remove_last_element(void) {
    PositionStorage s = PositionStorage_init();
    Entity e1 = entity_init(1, 1);
    Entity e2 = entity_init(2, 1);
    upsert_position(&s, e1, (Position){1, 1});
    upsert_position(&s, e2, (Position){2, 2});
    remove_position(&s, e2);
    TEST_ASSERT_EQUAL_UINT32(1, s.dense_count);
    TEST_ASSERT_TRUE(has_position(&s, e1));
    TEST_ASSERT_FALSE(has_position(&s, e2));
}

void test_remove_middle_swaps_last_in(void) {
    PositionStorage s = PositionStorage_init();
    Entity e1 = entity_init(1, 1);
    Entity e2 = entity_init(2, 1);
    Entity e3 = entity_init(3, 1);
    upsert_position(&s, e1, (Position){1, 1});
    upsert_position(&s, e2, (Position){2, 2});
    upsert_position(&s, e3, (Position){3, 3});
    remove_position(&s, e1);
    TEST_ASSERT_EQUAL_UINT32(2, s.dense_count);
    TEST_ASSERT_FALSE(has_position(&s, e1));
    TEST_ASSERT_TRUE(has_position(&s, e2));
    TEST_ASSERT_TRUE(has_position(&s, e3));
}

void test_remove_middle_swap_preserves_surviving_data(void) {
    PositionStorage s = PositionStorage_init();
    Entity e1 = entity_init(1, 1);
    Entity e2 = entity_init(2, 1);
    Entity e3 = entity_init(3, 1);
    upsert_position(&s, e1, (Position){10, 10});
    upsert_position(&s, e2, (Position){20, 20});
    upsert_position(&s, e3, (Position){30, 30});
    remove_position(&s, e1);
    uint32_t idx3 = s.sparse_entities[3];
    TEST_ASSERT_EQUAL_INT(30, s.dense_data[idx3].x);
    TEST_ASSERT_EQUAL_INT(30, s.dense_data[idx3].y);
    uint32_t idx2 = s.sparse_entities[2];
    TEST_ASSERT_EQUAL_INT(20, s.dense_data[idx2].x);
    TEST_ASSERT_EQUAL_INT(20, s.dense_data[idx2].y);
}

void test_remove_middle_updates_sparse_for_swapped_entity(void) {
    PositionStorage s = PositionStorage_init();
    Entity e1 = entity_init(1, 1);
    Entity e3 = entity_init(3, 1);
    upsert_position(&s, e1, (Position){10, 10});
    upsert_position(&s, entity_init(2, 1), (Position){20, 20});
    upsert_position(&s, e3, (Position){30, 30});
    remove_position(&s, e1);
    TEST_ASSERT_TRUE(s.sparse_entities[3] > 0);
    TEST_ASSERT_EQUAL_UINT32(0, s.sparse_entities[1]);
}
