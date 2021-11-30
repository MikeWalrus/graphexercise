#include <check.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../src/vec.h"

void test_push(struct Vec_int *v)
{
	for (int i = 0; i < 1024; i++) {
		vec_int_push(v, i);
		ck_assert_uint_eq(v->size, i + 1);
		ck_assert_uint_le(v->size, v->capacity);
	}
	for (size_t i = 0; i < 1024; i++) {
		ck_assert_int_eq(v->buf[i], i);
	}
}

START_TEST(test_vec)
{
	struct Vec_int v = vec_int_new();
	test_push(&v);
	for (size_t i = 0; i < 100; i++) {
		int val = vec_int_pop_back(&v);
		ck_assert_int_eq(val, 1023 - i);
		ck_assert_uint_eq(v.size, val);
	}
}
END_TEST

START_TEST(test_vec_with_cap_push)
{
	struct Vec_int v = vec_int_with_capacity(128);
	ck_assert_uint_eq(v.capacity, 128);
	test_push(&v);
	vec_int_delete(&v);
	v = vec_int_with_capacity(7);
	ck_assert_uint_eq(v.capacity, 7);
	for (size_t i = 0; i < 7; i++) {
		vec_int_push(&v, 0);
	}
	ck_assert_uint_eq(v.capacity, 7);
	vec_int_delete(&v);
}
END_TEST

Suite *vec_suite(void)
{
	Suite *s;
	TCase *tc;

	s = suite_create("Vec");

	tc = tcase_create("test");

	tcase_add_test(tc, test_vec);
	tcase_add_test(tc, test_vec_with_cap_push);
	suite_add_tcase(s, tc);

	return s;
}

int main(void)
{
	int number_failed;
	SRunner *sr;

	sr = srunner_create(vec_suite());

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
