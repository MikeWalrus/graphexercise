#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adjacency_list.h"
#include "adjacency_matrix.h"
#include "utilities.h"

static int *read_matrix(size_t matrix_len, char *str)
{
	char *end_ptr = str;
	int *edges = malloc(matrix_len * sizeof(*edges));
	for (size_t i = 0; i < matrix_len; i++) {
		const char *next = end_ptr;
		edges[i] = strtol(next, &end_ptr, 10);
		if (next == end_ptr) {
			if (*end_ptr == '_') {
				edges[i] = INT_MAX;
				end_ptr++;
			} else {
				free(edges);
				return NULL;
			}
		}
		end_ptr++;
	}
	return edges;
}

struct AdjMat *adj_mat_deserialise(const char *str)
{
	char *end_ptr;
	char *minus_sign = strchr(str, '-');
	size_t size = strtoul(str, &end_ptr, 10);
	if (end_ptr == str || (minus_sign && minus_sign < end_ptr)) {
		return NULL;
	}
	end_ptr++;
	if (!*end_ptr)
		return NULL;
	size_t matrix_len = size * size;

	int *edges = read_matrix(matrix_len, end_ptr);
	struct AdjMat *ret = malloc(sizeof(*ret));
	if (!ret || !edges) {
		free(edges);
		free(ret);
		return NULL;
	}
	ret->size = size;
	ret->edges = edges;
	ret->matrix_len = matrix_len;
	return ret;
}

size_t adj_mat_try_serialise(struct AdjMat *graph, char *buf, size_t buf_len)
{
	size_t len = 0;
	len += snprintf(NULL, 0, "%ld ", graph->size);
	for (size_t i = 0; i < graph->size; i++) {
		for (size_t j = 0; j < graph->size; j++) {
			len += snprintf(NULL, 0, "%d ",
			                graph->edges[i * graph->size + j]);
		}
	}
	return len;
}

char *adj_mat_serialise(struct AdjMat *graph)
{
	size_t len = PRINTED_MAX_CHARS(size_t) + 1 +
	             graph->matrix_len * (PRINTED_MAX_CHARS(int) + 1);
	char *ret = malloc(len);
	if (!ret)
		return NULL;

	char *p = ret;
	p += sprintf(p, "%ld ", graph->size);
	for (size_t i = 0; i < graph->matrix_len; i++) {
		if (graph->edges[i] != INT_MAX) {
			p += sprintf(p, "%d ", graph->edges[i]);
		} else {
			p += sprintf(p, "_ ");
		}
	}
	p--;
	*p = 0;
	return ret;
}

void adj_mat_delete(struct AdjMat *graph)
{
	if (graph) {
		free(graph->edges);
		free(graph);
	}
}

bool adj_mat_is_equal(const struct AdjMat *g1, const struct AdjMat *g2)
{
	if (g1->size != g2->size)
		return false;
	for (size_t i = 0; i < g1->matrix_len; i++) {
		if (g1->edges[i] != g2->edges[i])
			return false;
	}
	return true;
}

struct AdjMat *adj_mat_clone(const struct AdjMat *graph)
{
	struct AdjMat *ret = malloc(sizeof(*ret));
	*ret = (struct AdjMat){ .size = graph->size,
		                .matrix_len = graph->matrix_len,
		                .edges = malloc(graph->matrix_len *
		                                sizeof(*graph->edges)) };
	for (size_t i = 0; i < graph->matrix_len; i++)
		ret->edges[i] = graph->edges[i];
	return ret;
}

struct AdjMat *
adj_mat_from_adj_list_graph_weighted(const struct AdjListGraph *graph)
{
	struct AdjMat *ret = malloc(sizeof(*ret));
	size_t size = graph->size;
	ret->size = size;
	ret->matrix_len = size * size;
	ret->edges = malloc(ret->matrix_len * sizeof(*ret->edges));
	for (size_t i = 0; i < ret->matrix_len; i++) {
		ret->edges[i] = INT_MAX;
	}
	for (size_t i = 0; i < size; i++) {
		struct Vec_edge *edges = &graph->adj_lists[i];
		for (size_t j = 0; j < edges->size; j++) {
			size_t to = edges->buf[j].to;
			int weight = edges->buf[j].weight;
			ret->edges[i * size + to] = weight;
		}
	}
	return ret;
}
