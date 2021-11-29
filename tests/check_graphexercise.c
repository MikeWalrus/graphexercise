#include <check.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../src/adjacency_list.h"
#include "../src/adjacency_matrix.h"

START_TEST(test_adj_mat_deserialise)
{
	const char original_str[] = "5 "
				    "0 1 0 1 1 "
				    "1 0 1 1 0 "
				    "0 1 0 1 1 "
				    "1 1 1 0 1 "
				    "1 0 1 1 0";
	struct AdjMat *g = adj_mat_deserialise(original_str);
	ck_assert(g != NULL);
	ck_assert_int_eq(g->size, 5);
	char *serialised = adj_mat_serialise(g);
	ck_assert_str_eq(serialised, original_str);

	adj_mat_delete(g);
	free(serialised);
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

	tc_mat_ser = tcase_create("Serialisation");

	tcase_add_test(tc_mat_ser, test_adj_mat_deserialise);
	tcase_add_test(tc_mat_ser, test_adj_mat_deserialise_invalid);
	suite_add_tcase(s, tc_mat_ser);

	return s;
}

START_TEST(test_adj_list_graph_deserialise)
{
	const char original_str[] = "4\n"
				    "0 1 0 2 0 3 0\n"
				    "1 0 0 2 0 3 0\n"
				    "2 0 0 1 0\n"
				    "3\n";
	struct AdjListGraph *g = adj_list_graph_deserialise(original_str);
	ck_assert(g != NULL);
	char *serialised = adj_list_graph_serialise(g);
	ck_assert_str_eq(serialised, original_str);
	free(serialised);

	adj_list_graph_delete(g);
	adj_list_graph_deserialise("1\n0 1 0\n");
	ck_assert(g != NULL);
}
END_TEST

START_TEST(test_adj_list_graph_deserialise_invalid)
{
	struct AdjListGraph *g;
	g = adj_list_graph_deserialise("1");
	ck_assert_msg(
		g == NULL,
		"Should be invalid because it doesn't have adjacency list.");
	g = adj_list_graph_deserialise("1\n0 1 \n");
	ck_assert_msg(
		g == NULL,
		"Should be invalid because the weight of one of the edges isn't specified.");
}
END_TEST

Suite *adj_list_suite(void)
{
	Suite *s = suite_create("Adjacency List");
	TCase *tc_adj_list_ser = tcase_create("Serialisation");

	tcase_add_test(tc_adj_list_ser, test_adj_list_graph_deserialise);
	tcase_add_test(tc_adj_list_ser,
		       test_adj_list_graph_deserialise_invalid);
	suite_add_tcase(s, tc_adj_list_ser);
	return s;
}

int main(void)
{
	int number_failed;
	SRunner *sr;

	sr = srunner_create(adj_mat_suite());
	srunner_add_suite(sr, adj_list_suite());

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
