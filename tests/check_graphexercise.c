#include <check.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../src/adjacency_matrix.h"

START_TEST(test_adj_mat_deserialise)
{
	const char s[] = "5 "
			 "0 1 0 1 1 "
			 "1 0 1 1 0 "
			 "0 1 0 1 1 "
			 "1 1 1 0 1 "
			 "1 0 1 1 0";
	struct AdjMat *g = adj_mat_deserialise(s);
	ck_assert(g != NULL);
	ck_assert_int_eq(g->size, 5);
	char *serialised = adj_mat_serialise(g);
	bool identical = !strcmp(s, serialised);
	ck_assert(identical);
}
END_TEST

START_TEST(test_adj_mat_deserialise_invalid)
{
	struct AdjMat *m = adj_mat_deserialise("1"); // (no matrix)
	ck_assert_msg(m == NULL,
		      "Should be invalid because there is no matrix.");
	m = adj_mat_deserialise("1 "); // (with trailing space)
	ck_assert_msg(m == NULL,
		      "Should be invalid because there is no matrix.");
	m = adj_mat_deserialise("3 1 0"); // (not complete)
	ck_assert_msg(m == NULL,
		      "Should be invalid because the matrix is not complete.");
	m = adj_mat_deserialise("-2 1 0 0 1");
	ck_assert_msg(m == NULL,
		      "Should be invalid because the size is negative.");
}
END_TEST

Suite *adj_mat_suite(void)
{
	Suite *s;
	TCase *tc_mat_ser;

	s = suite_create("Adjacency Matrix");

	/* Core test case */
	tc_mat_ser = tcase_create("Serialisation");

	tcase_add_test(tc_mat_ser, test_adj_mat_deserialise);
	tcase_add_test(tc_mat_ser, test_adj_mat_deserialise_invalid);
	suite_add_tcase(s, tc_mat_ser);

	return s;
}

int main(void)
{
	int number_failed;
	Suite *s;
	SRunner *sr;

	s = adj_mat_suite();
	sr = srunner_create(s);

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
