#include <check.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/adjacency_list.h"
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

Suite *adj_list_suite(void)
{
	Suite *s;
	TCase *tc_mat;

	s = suite_create("Algorithms");

	tc_mat = tcase_create("test");

	tcase_add_test(tc_mat, test_dfs);
	suite_add_tcase(s, tc_mat);

	return s;
}

int main(void)
{
	int number_failed;
	SRunner *sr;

	sr = srunner_create(adj_list_suite());

	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);
	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
