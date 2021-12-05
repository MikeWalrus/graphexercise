#include "src/vec.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "adjacency_list.h"
#include "adjacency_matrix.h"
#include "utilities.h"

static struct AdjListGraph *adj_list_graph_new(size_t size)
{
	struct AdjListGraph *ret = malloc(sizeof(*ret));
	ret->size = size;
	ret->adj_lists = malloc(size * sizeof(*ret->adj_lists));
	for (size_t i = 0; i < size; i++) {
		ret->adj_lists[i] = vec_adj_edge_new();
	}
	return ret;
}

void adj_lists_delete(struct Vec_adj_edge *adj_lists, size_t size)
{
	for (size_t i = 0; i < size; i++) {
		vec_adj_edge_delete(&adj_lists[i]);
	}
	free(adj_lists);
}

static int edge_compare(const void *a, const void *b)
{
	const struct AdjEdge *edge1 = a;
	const struct AdjEdge *edge2 = b;
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
	if (end_ptr == str)
		return NULL;

	struct AdjListGraph *ret = adj_list_graph_new(size);

	for (size_t i = 0; i < size; i++) {
		char *next = end_ptr;

		// check whether the new line begins with vertex index
		if (i != strtol(next, &end_ptr, 10)) {
			adj_list_graph_delete(ret);
			return NULL;
		}

		struct Vec_adj_edge *edges = &ret->adj_lists[i];
		while (*end_ptr != '\n') {
			char *next = end_ptr;
			struct AdjEdge edge;
			edge.to = strtol(next, &end_ptr, 10);
			next = end_ptr;
			edge.weight = strtol(next, &end_ptr, 10);
			if (end_ptr == next) {
				adj_list_graph_delete(ret);
				return NULL;
			}
			vec_adj_edge_push(edges, edge);
		}
	}
	return ret;
}

void adj_list_graph_delete(struct AdjListGraph *graph)
{
	if (graph) {
		adj_lists_delete(graph->adj_lists, graph->size);
	}
	free(graph);
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
			struct AdjEdge *edge = &graph->adj_lists[i].buf[j];
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
		struct Vec_adj_edge *edges_1 = &g1->adj_lists[i];
		struct Vec_adj_edge *edges_2 = &g2->adj_lists[i];

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
		ret->adj_lists[i] = vec_adj_edge_clone(&graph->adj_lists[i]);
	}
	return ret;
}

struct AdjListGraph *adj_list_graph_from_adj_mat_weighted(struct AdjMat *graph)
{
	size_t size = graph->size;
	struct AdjListGraph *ret = adj_list_graph_new(size);
	for (size_t i = 0; i < size; i++) {
		struct Vec_adj_edge *edges = &ret->adj_lists[i];
		for (size_t j = 0; j < size; j++) {
			int weight = graph->edges[i * size + j];
			if (weight != INT_MAX) {
				vec_adj_edge_push(
					edges,
					(struct AdjEdge){ .to = j,
				                          .weight = weight });
			}
		}
	}
	return ret;
}

struct Vec_edge
adj_list_graph_undirected_get_edges(const struct AdjListGraph *graph)
{
	struct Vec_edge edges = vec_edge_new();
	for (size_t i = 0; i < graph->size; i++) {
		const struct Vec_adj_edge *adj_edges = &graph->adj_lists[i];
		for (size_t j = 0; j < adj_edges->size; j++) {
			struct AdjEdge e = adj_edges->buf[j];
			if (e.to > i) {
				struct Edge edge = { .a = i,
					             .b = j,
					             .weight = e.weight };
				vec_edge_push(&edges, edge);
			}
		}
	}
	return edges;
}
