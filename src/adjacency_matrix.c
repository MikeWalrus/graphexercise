#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "adjacency_matrix.h"
#include "utilities.h"

int *read_matrix(size_t size, char *str)
{
	char *end_ptr = str;
	int *edges = malloc(size * size * sizeof(*edges));
	for (size_t i = 0; i < size; i++) {
		for (size_t j = 0; j < size; j++) {
			const char *next = end_ptr;
			edges[i * size + j] = strtol(next, &end_ptr, 10);
			if (next == end_ptr) {
				free(edges);
				return NULL;
			}
			end_ptr++;
		}
	}
	return edges;
}

struct AdjMat *adj_mat_deserialise(const char *str)
{
	char *end_ptr;
	size_t size = strtol(str, &end_ptr, 10);
	if (end_ptr == str || size <= 0) {
		return NULL;
	}
	end_ptr++;
	if (!*end_ptr)
		return NULL;

	int *edges = read_matrix(size, end_ptr);
	struct AdjMat *ret = malloc(sizeof(*ret));
	if (!ret || !edges) {
		free(edges);
		free(ret);
		return NULL;
	}
	ret->size = size;
	ret->edges = edges;
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
	size_t size = graph->size;
	size_t matrix_len = size * size;
	size_t len = PRINTED_MAX_CHARS(size_t) + 1 +
		     matrix_len * (PRINTED_MAX_CHARS(int) + 1);
	char *ret = malloc(len);
	if (!ret)
		return NULL;

	char *p = ret;
	p += sprintf(p, "%ld ", graph->size);
	for (size_t i = 0; i < matrix_len; i++) {
		p += sprintf(p, "%d ", graph->edges[i]);
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
	size_t matrix_len = g1->size * g1->size;
	for (size_t i = 0; i < matrix_len; i++) {
		if (g1->edges[i] != g2->edges[i])
			return false;
	}
	return true;
}

struct AdjMat *adj_mat_clone(const struct AdjMat *graph)
{
	struct AdjMat *ret = malloc(sizeof(*ret));
	size_t matrix_len = graph->size * graph->size;
	*ret = (struct AdjMat){ .size = graph->size,
				.edges = malloc(matrix_len *
						sizeof(*graph->edges)) };
	for (size_t i = 0; i < matrix_len; i++)
		ret->edges[i] = graph->edges[i];
	return ret;
}
