#ifndef _MST_H
#define _MST_H

#include <stddef.h>

#include "adjacency_matrix.h"
#include "graph.h"

struct TwoVertices *mst_prim(struct AdjMat *graph, size_t *ret_size);

#endif
