#include <stdio.h>

#include "adjacency_list.h"
#include "utilities.h"

void adj_lists_delete(struct Vec_edge *adj_lists, size_t size)
{
	for (size_t i = 0; i < size; i++) {
		vec_edge_delete(&adj_lists[i]);
	}
	free(adj_lists);
}

static int edge_compare(const void *a, const void *b)
{
	const struct Edge *edge1 = a;
	const struct Edge *edge2 = b;
	size_t to1 = edge1->to;
	size_t to2 = edge2->to;
	if (to1 < to2)
		return -1;
	if (to1 == to2)
		return 0;
	return 1;
}

struct AdjListGraph *adj_list_graph_deserialise(const char *str)
{
	char *end_ptr;

	size_t size = strtol(str, &end_ptr, 10);
	if (end_ptr == str || size <= 0)
		return NULL;

	struct Vec_edge *adj_lists = malloc(size * sizeof(*adj_lists));
	for (size_t i = 0; i < size; i++) {
		adj_lists[i] = vec_edge_new();
	}

	for (size_t i = 0; i < size; i++) {
		char *next = end_ptr;

		// check whether the new line begins with vertex index
		if (i != strtol(next, &end_ptr, 10)) {
			adj_lists_delete(adj_lists, size);
			return NULL;
		}

		struct Vec_edge *edges = &adj_lists[i];
		while (*end_ptr != '\n') {
			char *next = end_ptr;
			struct Edge edge;
			edge.to = strtol(next, &end_ptr, 10);
			next = end_ptr;
			edge.weight = strtol(next, &end_ptr, 10);
			if (end_ptr == next) {
				adj_lists_delete(adj_lists, size);
				return NULL;
			}
			vec_edge_push(edges, edge);
		}
	}
	struct AdjListGraph *ret = malloc(sizeof(*ret));
	ret->adj_lists = adj_lists;
	ret->size = size;
	return ret;
}

void adj_list_graph_delete(struct AdjListGraph *graph)
{
	if (graph) {
		adj_lists_delete(graph->adj_lists, graph->size);
	}
}

char *adj_list_graph_serialise(struct AdjListGraph *graph)
{
	size_t max_len = 0;
	max_len += PRINTED_MAX_CHARS(size_t) + 1;
	for (size_t i = 0; i < graph->size; i++) {
		max_len += PRINTED_MAX_CHARS(size_t) + 1 +
			   (PRINTED_MAX_CHARS(size_t) + 1 +
			    PRINTED_MAX_CHARS(int) + 1) *
				   graph->adj_lists[i].size;
	}
	max_len++;
	char *ret = malloc(max_len);
	char *p = ret;
	p += sprintf(p, "%ld\n", graph->size);
	for (size_t i = 0; i < graph->size; i++) {
		p += sprintf(p, "%ld", i);
		for (size_t j = 0; j < graph->adj_lists[i].size; j++) {
			struct Edge *edge = &graph->adj_lists[i].buf[j];
			p += sprintf(p, " %ld %d", edge->to, edge->weight);
		}
		*(p++) = '\n';
	}
	return ret;
}

bool adj_list_graph_is_equal(struct AdjListGraph *g1, struct AdjListGraph *g2)
{
	if (g1->size != g2->size)
		return false;

	for (size_t i = 0; i < g1->size; i++) {
		struct Vec_edge *edges_1 = &g1->adj_lists[i];
		struct Vec_edge *edges_2 = &g2->adj_lists[i];

		if (edges_1->size != edges_2->size)
			return false;
		if (edges_1->size == 0)
			continue;
		qsort(edges_1->buf, edges_1->size, sizeof(*edges_1->buf),
		      edge_compare);
		qsort(edges_2->buf, edges_2->size, sizeof(*edges_2->buf),
		      edge_compare);
		for (size_t j = 0; j < edges_1->size; j++) {
			if (edges_1->buf[j].to != edges_2->buf[j].to ||
			    edges_1->buf[j].weight != edges_2->buf[j].weight)
				return false;
		}
	}
	return true;
}

struct AdjListGraph *adj_list_graph_clone(struct AdjListGraph *graph)
{
	struct AdjListGraph *ret = malloc(sizeof(*ret));
	ret->size = graph->size;
	ret->adj_lists = malloc(graph->size * sizeof(*ret->adj_lists));
	for (size_t i = 0; i < ret->size; i++) {
		ret->adj_lists[i] = vec_edge_clone(&graph->adj_lists[i]);
	}
	return ret;
}
