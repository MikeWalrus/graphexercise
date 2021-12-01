#include <check.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../src/queue.h"

START_TEST(test_queue)
{
	int n;
	struct Queue_int q = queue_int_new();
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

	tcase_add_test(tc, test_queue);
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
