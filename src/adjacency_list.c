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
		if (i != strtol(next, &end_ptr, 10)) {
			adj_lists_delete(adj_lists, size);
			return NULL;
		}

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
			vec_edge_push(&adj_lists[i], edge);
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
