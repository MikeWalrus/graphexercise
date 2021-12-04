#include <check.h>

#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../src/priority_queue.h"

struct PriorityQueue q;
const size_t size = 100;

void q_init(void)
{
	q = priority_queue_new(size);
}

void q_delete(void)
{
	priority_queue_delete(&q);
}

START_TEST(test_priority_queue_init_state)
{
	ck_assert_uint_eq(q.size, size);
	ck_assert_int_eq(q.heap[0].weight, INT_MIN);
	for (size_t i = 0; i < size; i++) {
		ck_assert_uint_eq(q.indices[i], i + 1);
	}
	for (size_t i = 1; i <= size; i++) {
		ck_assert_uint_eq(q.heap[i].index, i - 1);
		ck_assert_int_eq(q.heap[i].weight, INT_MAX);
	}
	ck_assert_int_eq(priority_queue_pop_min(&q).weight, INT_MAX);
	ck_assert_uint_eq(q.size, size - 1);
}
END_TEST

START_TEST(test_priority_queue_basic)
{
	bool improved;
	priority_queue_decrese_key(&q, 65, 1, &improved);
	ck_assert(improved);
	struct VertexPair min;
	min = priority_queue_pop_min(&q);
	ck_assert_uint_eq(min.index, 65);
	ck_assert_int_eq(min.weight, 1);
	ck_assert_uint_eq(q.size, size - 1);
}
END_TEST

START_TEST(test_priority_queue)
{
	size_t indices_list[] = { 34, 5,  2,  98, 99, 55, 6,  23, 48,
		                  1,  88, 45, 8,  77, 43, 26, 82, 21 };
	size_t len = sizeof(indices_list) / sizeof(*indices_list);
	for (size_t i = 0; i < len; i++) {
		bool improved;
		bool result = priority_queue_decrese_key(&q, indices_list[i], i,
		                                         &improved);
		ck_assert(result == true);
		ck_assert(improved);
	}
	for (size_t i = 0; i < len; i++) {
		bool improved;
		bool result = priority_queue_decrese_key(&q, indices_list[i], i,
		                                         &improved);
		ck_assert(result == true);
		ck_assert(!improved);
	}
	for (size_t i = 0; i < len; i++) {
		struct VertexPair min;
		min = priority_queue_pop_min(&q);
		ck_assert_uint_eq(min.index, indices_list[i]);
		ck_assert_int_eq(min.weight, i);
	}
	ck_assert_uint_eq(q.size, size - len);
}
END_TEST

Suite *priority_queue_suite(void)
{
	Suite *s;
	TCase *tc;

	s = suite_create("Priority Queue");

	tc = tcase_create("test");

	tcase_add_checked_fixture(tc, q_init, q_delete);

	tcase_add_test(tc, test_priority_queue_init_state);
	tcase_add_test(tc, test_priority_queue_basic);
	tcase_add_test(tc, test_priority_queue);
	suite_add_tcase(s, tc);

	return s;
}

int main(void)
{
	int number_failed;
	SRunner *sr;

	sr = srunner_create(priority_queue_suite());

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
