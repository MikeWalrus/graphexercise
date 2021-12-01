#include <check.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../src/queue.h"

START_TEST(test_queue1)
{
	struct Queue_int q = queue_int_new();
	for (size_t i = 0; i < 100; i++) {
		queue_int_push(&q, i);
		ck_assert_uint_eq(queue_int_len(&q), i + 1);
	}
	for (size_t i = 0; i < 100; i++) {
		int n = queue_int_pop_front(&q);
		ck_assert_int_eq(n, i);
	}
	for (size_t i = 0; i < 100; i++) {
		queue_int_push(&q, i);
		ck_assert_uint_eq(queue_int_len(&q), i + 1);
	}
	for (size_t i = 0; i < 100; i++) {
		int n = queue_int_pop_front(&q);
		ck_assert_int_eq(n, i);
	}
	ck_assert_uint_eq(queue_int_len(&q), 0);
}

START_TEST(test_queue2)
{
	int n;
	struct Queue_int q = queue_int_with_capacity(1);
	queue_int_push(&q, 200);
	queue_int_push(&q, 100);
	n = queue_int_pop_front(&q);
	ck_assert_int_eq(n, 200);
	queue_int_push(&q, 100);
	queue_int_pop_front(&q);
	queue_int_push(&q, 100);
	queue_int_pop_front(&q);
	queue_int_push(&q, 100);
	queue_int_pop_front(&q);
	queue_int_push(&q, 100);
	queue_int_push(&q, 100);
	queue_int_pop_front(&q);
	queue_int_push(&q, 100);
	queue_int_push(&q, 100);
	queue_int_pop_front(&q);
	queue_int_push(&q, 100);
	queue_int_pop_front(&q);
	queue_int_push(&q, 100);
	queue_int_pop_front(&q);
	ck_assert_int_eq(q.buf[q.head], 100);
}
END_TEST

Suite *queue_suite(void)
{
	Suite *s;
	TCase *tc;

	s = suite_create("Queue");

	tc = tcase_create("test");

	tcase_add_test(tc, test_queue1);
	tcase_add_test(tc, test_queue2);
	suite_add_tcase(s, tc);

	return s;
}

int main(void)
{
	int number_failed;
	SRunner *sr;

	sr = srunner_create(queue_suite());

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
