#include "src/vec.h"
#include <check.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../src/adjacency_list.h"
#include "../src/adjacency_matrix.h"

const char mat_str_1[] = "5 "
			 "0 1 0 1 1 "
			 "1 0 2 1 0 "
			 "0 _ _ _ 1 "
			 "1 1 1 0 1 "
			 "1 0 1 1 0";

const char weighted_graph_mat_str[] = "4 "
				      "_ 1 _ 3 "
				      "1 _ 2 _ "
				      "_ 2 _ 4 "
				      "3 _ 4 _";

const char weighted_graph_list_str[] = "4\n"
				       "0 1 1 3 3\n"
				       "1 0 1 2 2\n"
				       "2 1 2 3 4\n"
				       "3 0 3 2 4\n";

START_TEST(test_adj_mat_deserialise)
{
	struct AdjMat *g = adj_mat_deserialise(mat_str_1);
	ck_assert(g != NULL);
	ck_assert_int_eq(g->size, 5);
	char *serialised = adj_mat_serialise(g);
	ck_assert_str_eq(serialised, mat_str_1);

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

START_TEST(test_adj_mat_equality)
{
	struct AdjMat *g = adj_mat_deserialise(mat_str_1);
	struct AdjMat *g1 = adj_mat_deserialise(mat_str_1);
	ck_assert(adj_mat_is_equal(g, g1));
	struct AdjMat *g_clone = adj_mat_clone(g);
	ck_assert(adj_mat_is_equal(g_clone, g));
	adj_mat_delete(g);
	adj_mat_delete(g1);
	adj_mat_delete(g_clone);
}
END_TEST

START_TEST(test_adj_mat_get_edges)
{
	struct AdjMat *g = adj_mat_deserialise(weighted_graph_mat_str);
	struct Vec_edge edges = adj_mat_undirected_get_edges(g);
	ck_assert_uint_eq(edges.size, 4);
	vec_edge_delete(&edges);
	adj_mat_delete(g);
}
END_TEST

Suite *adj_mat_suite(void)
{
	Suite *s;
	TCase *tc_mat;

	s = suite_create("Adjacency Matrix");

	tc_mat = tcase_create("Serialisation");

	tcase_add_test(tc_mat, test_adj_mat_deserialise);
	tcase_add_test(tc_mat, test_adj_mat_deserialise_invalid);
	tcase_add_test(tc_mat, test_adj_mat_equality);
	tcase_add_test(tc_mat, test_adj_mat_get_edges);
	suite_add_tcase(s, tc_mat);

	return s;
}

const char adj_list_str_1[] = "4\n"
			      "0 1 0 2 0 3 0\n"
			      "1 0 0 2 0 3 0\n"
			      "2 0 0 1 0\n"
			      "3\n";

const char adj_list_str_2[] = "4\n"
			      "0 1 0 3 0\n"
			      "1 0 0 2 0 3 0\n"
			      "2 0 0 3 0\n"
			      "3\n";

START_TEST(test_adj_list_graph_deserialise)
{
	struct AdjListGraph *g = adj_list_graph_deserialise(adj_list_str_1);
	ck_assert(g != NULL);
	char *serialised = adj_list_graph_serialise(g);
	ck_assert_str_eq(serialised, adj_list_str_1);
	free(serialised);
	adj_list_graph_delete(g);

	g = adj_list_graph_deserialise("1\n0 1 0\n");
	ck_assert(g != NULL);
	adj_list_graph_delete(g);
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

START_TEST(test_adj_list_graph_equality)
{
	struct AdjListGraph *g = adj_list_graph_deserialise(adj_list_str_1);
	struct AdjListGraph *g1 = adj_list_graph_deserialise(adj_list_str_1);
	ck_assert(adj_list_graph_is_equal(g, g1));
	struct AdjListGraph *g_clone = adj_list_graph_clone(g);
	ck_assert(adj_list_graph_is_equal(g_clone, g));

	struct AdjListGraph *g2 = adj_list_graph_deserialise(adj_list_str_2);
	ck_assert(!adj_list_graph_is_equal(g2, g1));

	adj_list_graph_delete(g);
	adj_list_graph_delete(g1);
	adj_list_graph_delete(g2);
	adj_list_graph_delete(g_clone);
}
END_TEST

START_TEST(test_adj_list_get_edges)
{
	struct AdjMat *g = adj_mat_deserialise(weighted_graph_mat_str);
	struct AdjListGraph *g_list = adj_list_graph_from_adj_mat_weighted(g);
	adj_mat_delete(g);
	struct Vec_edge edges = adj_list_graph_undirected_get_edges(g_list);
	ck_assert_uint_eq(edges.size, 4);
	vec_edge_delete(&edges);
	adj_list_graph_delete(g_list);
}
END_TEST

Suite *adj_list_suite(void)
{
	Suite *s = suite_create("Adjacency List");
	TCase *tc_adj_list = tcase_create("Serialisation");

	tcase_add_test(tc_adj_list, test_adj_list_graph_deserialise);
	tcase_add_test(tc_adj_list, test_adj_list_graph_deserialise_invalid);
	tcase_add_test(tc_adj_list, test_adj_list_graph_equality);
	tcase_add_test(tc_adj_list, test_adj_list_get_edges);
	suite_add_tcase(s, tc_adj_list);
	return s;
}

struct AdjMat *adj_mat_graph;

START_TEST(test_mat_list_conversion)
{
	struct AdjMat *g_mat = adj_mat_deserialise(weighted_graph_mat_str);
	ck_assert(g_mat != NULL);
	struct AdjListGraph *g_list =
		adj_list_graph_deserialise(weighted_graph_list_str);
	ck_assert(g_list != NULL);
	struct AdjMat *g_mat_from_list =
		adj_mat_from_adj_list_graph_weighted(g_list);
	ck_assert(adj_mat_is_equal(g_mat_from_list, g_mat));
	struct AdjListGraph *g_list_from_mat =
		adj_list_graph_from_adj_mat_weighted(g_mat);
	ck_assert(g_list_from_mat != NULL);
	ck_assert(adj_list_graph_is_equal(g_list, g_list_from_mat));

	adj_mat_delete(g_mat);
	adj_mat_delete(g_mat_from_list);
	adj_list_graph_delete(g_list);
	adj_list_graph_delete(g_list_from_mat);
}
END_TEST

Suite *conversion_suite(void)
{
	Suite *s = suite_create("Adjacency List/Matrix Conversion");
	TCase *tc = tcase_create("test");

	tcase_add_test(tc, test_mat_list_conversion);
	suite_add_tcase(s, tc);
	return s;
}

void setup_adj_mat_graph(void)
{
	adj_mat_graph = adj_mat_deserialise(mat_str_1);
}

void teardown_adj_mat_graph(void)
{
	adj_mat_delete(adj_mat_graph);
}

int main(void)
{
	int number_failed;
	SRunner *sr;

	sr = srunner_create(adj_mat_suite());
	srunner_add_suite(sr, adj_list_suite());
	srunner_add_suite(sr, conversion_suite());

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
