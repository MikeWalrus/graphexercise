#include "src/adjacency_matrix.h"
#include <check.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/adjacency_list.h"
#include "../src/adjacency_matrix.h"
#include "../src/mst.h"
#include "../src/shortest_path.h"
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
const char connected_weighted_graph_1_str[] = "6 "
					      "_ 1 _ 4 3 _ "
					      "1 _ _ 4 2 _ "
					      "_ _ _ _ 4 5 "
					      "4 4 _ _ 4 _ "
					      "3 2 4 4 _ 7 "
					      "_ _ 5 _ 7 _";
const size_t connected_weighted_graph_1_mst_weight = 16;

struct AdjMat *connected_weighted_graph_1;

void connected_weighted_graph_1_init(void)
{
	connected_weighted_graph_1 =
		adj_mat_deserialise(connected_weighted_graph_1_str);
	ck_assert(connected_weighted_graph_1);
}

void connected_weighted_graph_1_delete(void)
{
	adj_mat_delete(connected_weighted_graph_1);
}

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
	int weight = mst_weight(connected_weighted_graph_1, mst_prim);
	ck_assert_int_eq(weight, connected_weighted_graph_1_mst_weight);
}
END_TEST

START_TEST(test_kruskal)
{
	int weight = mst_weight(connected_weighted_graph_1, mst_prim);
	ck_assert_int_eq(weight, connected_weighted_graph_1_mst_weight);
}
END_TEST

struct AdjMat *connected_weighted_graph_real;

void connected_weighted_graph_real_load(void)
{
	struct AdjListGraph *tmp =
		graph_load(DATASET_DIR "USAir97_adj_list.txt");
	ck_assert(tmp);
	connected_weighted_graph_real =
		adj_mat_from_adj_list_graph_weighted(tmp);
	ck_assert(connected_weighted_graph_real);
	adj_list_graph_delete(tmp);
}

void connected_weighted_graph_real_delete(void)
{
	adj_mat_delete(connected_weighted_graph_real);
}

START_TEST(test_mst)
{
	long long weight_prim =
		mst_weight(connected_weighted_graph_real, mst_prim);
	long long weight_kruskal =
		mst_weight(connected_weighted_graph_real, mst_kruskal_adj_mat);
	ck_assert_int_eq(weight_prim, weight_kruskal);
}
END_TEST

Suite *mst_suite(void)
{
	Suite *s;
	TCase *tc_basic;
	TCase *tc_real;

	s = suite_create("Minimum Spanning Tree");

	tc_basic = tcase_create("basic");
	tcase_add_checked_fixture(tc_basic, connected_weighted_graph_1_init,
	                          connected_weighted_graph_1_delete);
	tcase_add_test(tc_basic, test_prim);
	tcase_add_test(tc_basic, test_kruskal);

	tc_real = tcase_create("real");
	tcase_add_checked_fixture(tc_real, connected_weighted_graph_real_load,
	                          connected_weighted_graph_real_delete);
	tcase_add_test(tc_real, test_mst);

	suite_add_tcase(s, tc_basic);
	suite_add_tcase(s, tc_real);
	return s;
}

START_TEST(test_dijkstra)
{
	struct ShortestPathResult *r =
		shortest_path_dijkstra(connected_weighted_graph_1, 0);
	struct ShortestPathResult expected[6];
	expected[0].weight = 0;
	expected[1].weight = 1;
	expected[2].weight = 7;
	expected[3].weight = 4;
	expected[4].weight = 3;
	expected[5].weight = 10;
	for (size_t i = 0; i < 6; i++)
		ck_assert_int_eq(expected[i].weight, r[i].weight);
	free(r);
}
END_TEST

START_TEST(test_floyd_warshall)
{
	struct ShortestPathResult *r =
		shortest_path_floyd_warshall(connected_weighted_graph_1);
	free(r);
}
END_TEST

Suite *shortest_path_suite(void)
{
	Suite *s;
	TCase *tc_basic;

	s = suite_create("Shortest Path");

	tc_basic = tcase_create("basic");
	tcase_add_checked_fixture(tc_basic, connected_weighted_graph_1_init,
	                          connected_weighted_graph_1_delete);
	tcase_add_test(tc_basic, test_dijkstra);
	tcase_add_test(tc_basic, test_floyd_warshall);

	suite_add_tcase(s, tc_basic);
	return s;
}

int main(void)
{
	int number_failed;
	SRunner *sr;

	sr = srunner_create(traversal_suite());
	srunner_add_suite(sr, mst_suite());
	srunner_add_suite(sr, shortest_path_suite());

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
