#ifndef _ADJACENCY_LIST_H
#define _ADJACENCY_LIST_H

#include <stdbool.h>
#include <stddef.h>

#include "vec.h"

struct Edge {
	size_t to;
	int weight;
};

DECL_VEC(struct Edge, edge);

struct AdjListGraph {
	size_t size;
	struct Vec_edge *adj_lists;
};

struct AdjListGraph *adj_list_graph_deserialise(const char *str);

void adj_list_graph_delete(struct AdjListGraph *graph);

char *adj_list_graph_serialise(struct AdjListGraph *graph);

bool adj_list_graph_is_equal(struct AdjListGraph *g1, struct AdjListGraph *g2);

struct AdjListGraph *adj_list_graph_clone(struct AdjListGraph *graph);

#endif
