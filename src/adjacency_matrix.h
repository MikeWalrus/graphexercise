#ifndef ADJACENCY_MATRIX_H
#define ADJACENCY_MATRIX_H

#include <stddef.h>

struct Vertex {
	int value;
};

struct AdjMat {
	int *edges;
	struct Vertex *vetices;
	size_t size;
};

struct AdjMat *adj_mat_deserialise(const char *str);

char *adj_mat_serialise(struct AdjMat *graph);

void adj_mat_delete(struct AdjMat *graph);

#endif
