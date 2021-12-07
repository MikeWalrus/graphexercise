#include "src/adjacency_list.h"
#include "src/vec.h"
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

#include "adjacency_matrix.h"
#include "disjoint_set.h"
#include "graph.h"
#include "mst.h"
#include "priority_queue.h"

void prim_update_adjacent_vertices(size_t curr, struct AdjMat *graph,
                                   struct PriorityQueue *q, size_t *from)
{
	size_t size = graph->size;
	for (size_t vertex = 0; vertex < size; vertex++) {
		int weight = graph->edges[curr * size + vertex];
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
	priority_queue_delete(&q);
	return ret;
}

struct TwoVertices *mst_kruskal(size_t graph_size, struct Vec_edge *edges,
                                size_t *ret_size)
{
	*ret_size = graph_size - 1;
	struct TwoVertices *ret = malloc(*ret_size * sizeof(*ret));
	struct TwoVertices *output_ptr = ret;

	struct DisjointSet disjoint_set = disjoint_set_new(graph_size);

	vec_edge_sort(edges, edge_compare_by_weight);
	for (size_t i = 0; i < edges->size; i++) {
		struct Edge edge = edges->buf[i];
		if (!disjoint_set_is_in_same_set(&disjoint_set, edge.a,
		                                 edge.b)) {
			disjoint_set_union(&disjoint_set, edge.a, edge.b);
			*(output_ptr++) = (struct TwoVertices){ .a = edge.a,
				                                .b = edge.b };
		}
	}
	disjoint_set_delete(&disjoint_set);
	return ret;
}

struct TwoVertices *mst_kruskal_adj_mat(struct AdjMat *graph, size_t *ret_size)
{
	struct Vec_edge edges = adj_mat_undirected_get_edges(graph);
	struct TwoVertices *ret = mst_kruskal(graph->size, &edges, ret_size);
	vec_edge_delete(&edges);
	return ret;
}

struct TwoVertices *mst_kruskal_adj_list_graph(struct AdjListGraph *graph,
                                               size_t *ret_size)
{
	struct Vec_edge edges = adj_list_graph_undirected_get_edges(graph);
	struct TwoVertices *ret = mst_kruskal(graph->size, &edges, ret_size);
	vec_edge_delete(&edges);
	return ret;
}
