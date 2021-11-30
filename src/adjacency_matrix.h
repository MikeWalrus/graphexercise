#ifndef ADJACENCY_MATRIX_H
#define ADJACENCY_MATRIX_H

#include <stdbool.h>
#include <stddef.h>

struct AdjMat {
	int *edges;
	size_t size;
};

struct AdjMat *adj_mat_deserialise(const char *str);

char *adj_mat_serialise(struct AdjMat *graph);

void adj_mat_delete(struct AdjMat *graph);

bool adj_mat_is_equal(const struct AdjMat *g1, const struct AdjMat *g2);

struct AdjMat *adj_mat_clone(const struct AdjMat *graph);

#endif
