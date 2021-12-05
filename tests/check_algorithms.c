#include "src/adjacency_matrix.h"
#include <check.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/adjacency_list.h"
#include "../src/adjacency_matrix.h"
#include "../src/mst.h"
#include "../src/traverse.h"

struct AdjListGraph *graph_load(const char *filename)
{
	FILE *f;
	char *buf;
	size_t file_len;

	f = fopen(filename, "r");
	ck_assert_msg(f != NULL, "Failed to open the file.");

	fseek(f, 0L, SEEK_END);
	file_len = ftell(f);
	fseek(f, 0L, SEEK_SET);

	buf = malloc(file_len);
	ck_assert_msg(buf != NULL, "Malloc failed.");
	fread(buf, 1, file_len, f);
	fclose(f);

	struct AdjListGraph *ret = adj_list_graph_deserialise(buf);
	ck_assert(ret != NULL);
	free(buf);
	return ret;
}

void test_traversal_trace_on(struct AdjListGraph *graph, size_t *trace,
                             size_t trace_size)
{
	ck_assert_uint_eq(trace_size, graph->size);
	bool *visited = malloc(sizeof(*visited) * graph->size);
	for (size_t i = 0; i < graph->size; i++) {
		visited[i] = false;
	}
	for (size_t i = 0; i < trace_size; i++) {
		size_t vertex_index = trace[i];
		ck_assert(!visited[vertex_index]);
		visited[vertex_index] = true;
	}
	for (size_t i = 0; i < graph->size; i++) {
		ck_assert(visited[i]);
	}
	free(visited);
}

void test_dfs_trace(struct AdjListGraph *graph)
{
	size_t trace_size;
	size_t *trace = dfs_adj_list_graph(graph, &trace_size);
	test_traversal_trace_on(graph, trace, trace_size);
	free(trace);
}

void test_bfs_trace(struct AdjListGraph *graph)
{
	size_t trace_size;
	size_t *trace = bfs_adj_list_graph(graph, &trace_size);
	test_traversal_trace_on(graph, trace, trace_size);
	free(trace);
}

START_TEST(test_dfs)
{
	struct AdjListGraph *graph;
	graph = adj_list_graph_deserialise("4\n"
	                                   "0 1 0 2 0 3 0\n"
	                                   "1 0 0 2 0 3 0\n"
	                                   "2 0 0 1 0\n"
	                                   "3\n");
	test_dfs_trace(graph);
	adj_list_graph_delete(graph);
	graph = graph_load(DATASET_DIR "lastfm_adj_list.txt");
	test_dfs_trace(graph);
	adj_list_graph_delete(graph);
}
END_TEST

START_TEST(test_bfs)
{
	struct AdjListGraph *graph;
	graph = adj_list_graph_deserialise("4\n"
	                                   "0 1 0 2 0 3 0\n"
	                                   "1 0 0 2 0 3 0\n"
	                                   "2 0 0 1 0\n"
	                                   "3\n");
	test_bfs_trace(graph);
	adj_list_graph_delete(graph);
	graph = graph_load(DATASET_DIR "lastfm_adj_list.txt");
	test_bfs_trace(graph);
	adj_list_graph_delete(graph);
}
END_TEST

Suite *traversal_suite(void)
{
	Suite *s;
	TCase *tc;

	s = suite_create("Traversal");

	tc = tcase_create("test");

	tcase_add_test(tc, test_dfs);
	tcase_add_test(tc, test_bfs);
	suite_add_tcase(s, tc);

	return s;
}

// https://commons.wikimedia.org/wiki/File:Msp1.jpg
const char connected_weighted_graph_1[] = "6 "
					  "_ 1 _ 4 3 _ "
					  "1 _ _ 4 2 _ "
					  "_ _ _ _ 4 5 "
					  "4 4 _ _ 4 _ "
					  "3 2 4 4 _ 7 "
					  "_ _ 5 _ 7 _";
const size_t connected_weighted_graph_1_mst_weight = 16;

int mst_weight(struct AdjMat *g,
               struct TwoVertices *(*mst)(struct AdjMat *graph,
                                          size_t *ret_size))
{
	size_t mst_edges_size;
	struct TwoVertices *mst_edges = mst_kruskal_adj_mat(g, &mst_edges_size);
	int weight = adj_mat_edges_weight_sum(g, mst_edges, mst_edges_size);
	free(mst_edges);
	return weight;
}

START_TEST(test_prim)
{
	struct AdjMat *g = adj_mat_deserialise(connected_weighted_graph_1);
	ck_assert(g);
	int weight = mst_weight(g, mst_prim);
	ck_assert_int_eq(weight, connected_weighted_graph_1_mst_weight);

	adj_mat_delete(g);
}
END_TEST

START_TEST(test_kruskal)
{
	struct AdjMat *g = adj_mat_deserialise(connected_weighted_graph_1);
	ck_assert(g);
	int weight = mst_weight(g, mst_prim);
	ck_assert_int_eq(weight, connected_weighted_graph_1_mst_weight);

	adj_mat_delete(g);
}
END_TEST

START_TEST(test_mst)
{
	struct AdjListGraph *g_list =
		graph_load(DATASET_DIR "USAir97_adj_list.txt");
	struct AdjMat *g_mat = adj_mat_from_adj_list_graph_weighted(g_list);
	adj_list_graph_delete(g_list);
	ck_assert(g_mat != NULL);
	long long weight_prim = mst_weight(g_mat, mst_prim);
	long long weight_kruskal = mst_weight(g_mat, mst_kruskal_adj_mat);
	ck_assert_int_eq(weight_prim, weight_kruskal);
	adj_mat_delete(g_mat);
}
END_TEST

Suite *mst_suite(void)
{
	Suite *s;
	TCase *tc;

	s = suite_create("Minimum Spanning Tree");

	tc = tcase_create("test");

	tcase_add_test(tc, test_prim);
	tcase_add_test(tc, test_kruskal);
	tcase_add_test(tc, test_mst);
	suite_add_tcase(s, tc);

	return s;
}

int main(void)
{
	int number_failed;
	SRunner *sr;

	sr = srunner_create(traversal_suite());
	srunner_add_suite(sr, mst_suite());

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
