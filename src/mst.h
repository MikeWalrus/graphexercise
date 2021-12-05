#ifndef _MST_H
#define _MST_H

#include <stddef.h>

#include "adjacency_matrix.h"
#include "graph.h"

struct TwoVertices *mst_prim(struct AdjMat *graph, size_t *ret_size);

struct TwoVertices *mst_kruskal_adj_mat(struct AdjMat *graph, size_t *ret_size);

struct TwoVertices *mst_kruskal(size_t graph_size, struct Vec_edge *edges,
                                size_t *ret_size);

struct TwoVertices *mst_kruskal_adj_list_graph(struct AdjListGraph *graph,
                                               size_t *ret_size);

#endif
