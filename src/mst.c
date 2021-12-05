#include <bits/posix1_lim.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

#include "adjacency_matrix.h"
#include "mst.h"
#include "priority_queue.h"

void prim_update_adjacent_vertices(size_t curr, struct AdjMat *graph,
                                   struct PriorityQueue *q, size_t *from)
{
	size_t size = graph->size;
	for (size_t vertex = 0; vertex < size; vertex++) {
		size_t weight = graph->edges[curr * size + vertex];
		bool is_adjacent = weight != INT_MAX;
		if (is_adjacent) {
			bool improved;
			bool visited = !priority_queue_decrese_key(
				q, vertex, weight, &improved);
			if (!visited && improved)
				from[vertex] = curr;
		}
	}
}

struct TwoVertices *mst_prim(struct AdjMat *graph, size_t *ret_size)
{
	size_t size = graph->size;
	*ret_size = size - 1;
	struct TwoVertices *ret = malloc(*ret_size * sizeof(*ret));
	struct TwoVertices *output_ptr = ret;

	struct PriorityQueue q = priority_queue_new(size);
	size_t *from = malloc(size * sizeof(*from));

	bool improved;
	priority_queue_decrese_key(&q, 0, 0, &improved);
	size_t curr = priority_queue_pop_min(&q).index;

	for (;;) {
		prim_update_adjacent_vertices(curr, graph, &q, from);
		if (q.size <= 0) {
			break;
		}
		curr = priority_queue_pop_min(&q).index;
		*(output_ptr++) =
			(struct TwoVertices){ .a = from[curr], .b = curr };
	}
	free(from);
	return ret;
}
