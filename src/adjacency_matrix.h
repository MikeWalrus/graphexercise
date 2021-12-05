#ifndef ADJACENCY_MATRIX_H
#define ADJACENCY_MATRIX_H

#include <stdbool.h>
#include <stddef.h>

#include "graph.h"
#include "vec.h"

struct AdjMat {
	int *edges;
	size_t size;
	size_t matrix_len;
};

struct AdjMat *adj_mat_deserialise(const char *str);

char *adj_mat_serialise(struct AdjMat *graph);

void adj_mat_delete(struct AdjMat *graph);

bool adj_mat_is_equal(const struct AdjMat *g1, const struct AdjMat *g2);

struct AdjMat *adj_mat_clone(const struct AdjMat *graph);

struct AdjListGraph;

struct AdjMat *
adj_mat_from_adj_list_graph_weighted(const struct AdjListGraph *graph);

long long adj_mat_edges_weight_sum(const struct AdjMat *graph,
                                   struct TwoVertices *edges,
                                   size_t edges_size);

struct Vec_edge adj_mat_undirected_get_edges(const struct AdjMat *graph);

#endif
