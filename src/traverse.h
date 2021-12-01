#ifndef _TRAVERS_H
#define _TRAVERS_H

#include "adjacency_list.h"
#include "vec.h"

size_t *dfs_adj_list_graph(const struct AdjListGraph *graph, size_t *ret_size);

#endif
