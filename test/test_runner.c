#include "unity.h"

extern void test_entity_init_packs_generation_and_index(void);
extern void test_entity_init_with_zero_values(void);
extern void test_is_entity_valid_returns_false_for_zero(void);
extern void test_is_entity_valid_returns_true_for_nonzero(void);
extern void test_init_entity_manager_sets_all_dead(void);
extern void test_init_entity_manager_sets_zero_generations(void);
extern void test_init_entity_manager_free_count_equals_max(void);
extern void test_create_entity_marks_alive(void);
extern void test_create_entity_decrements_free_count(void);
extern void test_create_entity_has_generation_zero(void);
extern void test_entity_alive_returns_true_for_live_entity(void);
extern void test_destroy_entity_marks_dead(void);
extern void test_destroy_entity_increments_generation(void);
extern void test_entity_alive_returns_false_after_destroy(void);
extern void test_destroy_entity_increments_free_count(void);
extern void test_reused_entity_gets_new_generation(void);
extern void test_old_entity_handle_not_alive_after_reuse(void);
extern void test_destroy_already_dead_entity_is_noop(void);

extern void test_init_dense_count_is_zero(void);
extern void test_init_sparse_all_zero(void);
extern void test_init_dense_data_all_zero(void);
extern void test_init_dense_entities_all_zero(void);
extern void test_has_position_false_after_init(void);
extern void test_upsert_increments_dense_count(void);
extern void test_upsert_stores_entity_in_dense(void);
extern void test_upsert_stores_data_in_dense(void);
extern void test_upsert_updates_sparse(void);
extern void test_upsert_multiple_entities(void);
extern void test_has_position_true_after_upsert(void);
extern void test_has_position_false_for_wrong_generation(void);
extern void test_has_position_false_for_different_index(void);
extern void test_upsert_update_does_not_increment_dense_count(void);
extern void test_upsert_update_changes_data(void);
extern void test_remove_makes_has_position_false(void);
extern void test_remove_decrements_dense_count(void);
extern void test_remove_clears_sparse(void);
extern void test_remove_nonexistent_is_noop(void);
extern void test_remove_last_element(void);
extern void test_remove_middle_swaps_last_in(void);
extern void test_remove_middle_swap_preserves_surviving_data(void);
extern void test_remove_middle_updates_sparse_for_swapped_entity(void);

void setUp(void) {}
void tearDown(void) {}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_entity_init_packs_generation_and_index);
    RUN_TEST(test_entity_init_with_zero_values);
    RUN_TEST(test_is_entity_valid_returns_false_for_zero);
    RUN_TEST(test_is_entity_valid_returns_true_for_nonzero);
    RUN_TEST(test_init_entity_manager_sets_all_dead);
    RUN_TEST(test_init_entity_manager_sets_zero_generations);
    RUN_TEST(test_init_entity_manager_free_count_equals_max);
    RUN_TEST(test_create_entity_marks_alive);
    RUN_TEST(test_create_entity_decrements_free_count);
    RUN_TEST(test_create_entity_has_generation_zero);
    RUN_TEST(test_entity_alive_returns_true_for_live_entity);
    RUN_TEST(test_destroy_entity_marks_dead);
    RUN_TEST(test_destroy_entity_increments_generation);
    RUN_TEST(test_entity_alive_returns_false_after_destroy);
    RUN_TEST(test_destroy_entity_increments_free_count);
    RUN_TEST(test_reused_entity_gets_new_generation);
    RUN_TEST(test_old_entity_handle_not_alive_after_reuse);
    RUN_TEST(test_destroy_already_dead_entity_is_noop);

    RUN_TEST(test_init_dense_count_is_zero);
    RUN_TEST(test_init_sparse_all_zero);
    RUN_TEST(test_init_dense_data_all_zero);
    RUN_TEST(test_init_dense_entities_all_zero);
    RUN_TEST(test_has_position_false_after_init);
    RUN_TEST(test_upsert_increments_dense_count);
    RUN_TEST(test_upsert_stores_entity_in_dense);
    RUN_TEST(test_upsert_stores_data_in_dense);
    RUN_TEST(test_upsert_updates_sparse);
    RUN_TEST(test_upsert_multiple_entities);
    RUN_TEST(test_has_position_true_after_upsert);
    RUN_TEST(test_has_position_false_for_wrong_generation);
    RUN_TEST(test_has_position_false_for_different_index);
    RUN_TEST(test_upsert_update_does_not_increment_dense_count);
    RUN_TEST(test_upsert_update_changes_data);
    RUN_TEST(test_remove_makes_has_position_false);
    RUN_TEST(test_remove_decrements_dense_count);
    RUN_TEST(test_remove_clears_sparse);
    RUN_TEST(test_remove_nonexistent_is_noop);
    RUN_TEST(test_remove_last_element);
    RUN_TEST(test_remove_middle_swaps_last_in);
    RUN_TEST(test_remove_middle_swap_preserves_surviving_data);
    RUN_TEST(test_remove_middle_updates_sparse_for_swapped_entity);

    return UNITY_END();
}
