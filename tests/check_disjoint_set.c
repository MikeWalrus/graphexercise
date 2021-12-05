#include <check.h>

#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../src/disjoint_set.h"

#define LEN(a) sizeof(a) / sizeof(*a)

struct DisjointSet s;
const size_t size = 12;

void s_init(void)
{
	s = disjoint_set_new(size);
}

void s_delete(void)
{
	disjoint_set_delete(&s);
}

void check_not_same_set(struct DisjointSet *s, size_t *elems, size_t elems_size)
{
	for (size_t i = 0; i < elems_size; i++) {
		for (size_t j = 0; j < elems_size; j++) {
			bool same_set = disjoint_set_is_in_same_set(s, elems[i],
			                                            elems[j]);
			if (i == j)
				ck_assert(same_set);
			else
				ck_assert(!same_set);
		}
	}
}

void check_same_set(struct DisjointSet *s, size_t *elems, size_t elems_size)
{
	for (size_t i = 0; i < elems_size; i++) {
		for (size_t j = 0; j < elems_size; j++) {
			bool same_set = disjoint_set_is_in_same_set(s, elems[i],
			                                            elems[j]);
			ck_assert(same_set);
		}
	}
}

START_TEST(test_disjoint_set_init_state)
{
	ck_assert_uint_eq(s.size, size);
	size_t not_in_same_set[size];
	for (size_t i = 0; i < size; i++)
		not_in_same_set[i] = i;
	check_not_same_set(&s, not_in_same_set, size);
}
END_TEST

START_TEST(test_disjoint_set_basic)
{
	disjoint_set_union(&s, 1, 2);
	disjoint_set_union(&s, 6, 5);
	disjoint_set_union(&s, 1, 5);
	disjoint_set_union(&s, 6, 8);
	disjoint_set_union(&s, 3, 4);
	size_t set1[] = { 1, 2, 5, 6, 8 };
	size_t set2[] = { 3, 4 };
	size_t not_same_set1[] = { 1, 3, 7 };
	size_t not_same_set2[] = { 2, 4, 7 };
	check_same_set(&s, set1, LEN(set1));
	check_same_set(&s, set2, LEN(set2));
	check_not_same_set(&s, not_same_set1, LEN(not_same_set1));
	check_not_same_set(&s, not_same_set2, LEN(not_same_set2));
}
END_TEST

Suite *disjoint_set_suite(void)
{
	Suite *s;
	TCase *tc;

	s = suite_create("Disjoint-set");

	tc = tcase_create("test");

	tcase_add_checked_fixture(tc, s_init, s_delete);

	tcase_add_test(tc, test_disjoint_set_init_state);
	tcase_add_test(tc, test_disjoint_set_basic);
	suite_add_tcase(s, tc);

	return s;
}

int main(void)
{
	int number_failed;
	SRunner *sr;

	sr = srunner_create(disjoint_set_suite());

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
