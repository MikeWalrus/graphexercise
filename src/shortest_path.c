#include "src/vec.h"
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

#include "shortest_path.h"

void dijkstra_update_adjacent_vertices(size_t curr, struct AdjMat *graph,
                                       struct PriorityQueue *q,
                                       struct ShortestPathResult *result)
{
	size_t size = graph->size;
	for (size_t vertex = 0; vertex < size; vertex++) {
		int edge_weight = graph->edges[curr * size + vertex];
		bool is_adjacent = edge_weight != INT_MAX;
		if (is_adjacent) {
			int weight = edge_weight + result[curr].weight;
			bool improved;
			bool visited = !priority_queue_decrese_key(
				q, vertex, weight, &improved);
			if (!visited && improved)
				result[vertex] = (struct ShortestPathResult){
					.next = curr, .weight = weight
				};
		}
	}
}

struct ShortestPathResult *shortest_path_dijkstra(struct AdjMat *graph,
                                                  size_t vertex_index)
{
	size_t size = graph->size;
	struct ShortestPathResult *ret = malloc(size * sizeof(*ret));
	ret[vertex_index].weight = 0;

	struct PriorityQueue q = priority_queue_new(size);

	bool improved;
	priority_queue_decrese_key(&q, vertex_index, 0, &improved);
	size_t curr = vertex_index;
	for (;;) {
		dijkstra_update_adjacent_vertices(curr, graph, &q, ret);
		if (q.size <= 0)
			break;
		struct VertexPair min = priority_queue_pop_min(&q);
		curr = min.index;
	}
	priority_queue_delete(&q);
	return ret;
}

struct ShortestPathResult *shortest_path_floyd_warshall(struct AdjMat *graph)
{
	size_t size = graph->size;
	struct ShortestPathResult *result =
		malloc(graph->matrix_len * sizeof(*result));
	for (size_t i = 0; i < graph->size; i++) {
		for (size_t j = 0; j < graph->size; j++) {
			int weight = graph->edges[i * size + j];
			result[i * size + j].weight = weight;
			if (weight != INT_MAX)
				result[i * size + j].next = j;
			else
				result[i * size + j].next = SIZE_MAX;
		}
	}

	for (size_t k = 0; k < size; k++) {
		for (size_t i = 0; i < size; i++) {
			for (size_t j = 0; j < size; j++) {
				if (result[k * size + j].weight == INT_MAX ||
				    result[i * size + k].weight == INT_MAX) {
					continue;
				}
				int dist_through_k =
					result[k * size + j].weight +
					result[i * size + k].weight;
				if (dist_through_k <
				    result[i * size + j].weight) {
					result[i * size + j].weight =
						result[k * size + j].weight +
						result[i * size + k].weight;
					result[i * size + j].next =
						result[i * size + k].next;
				}
			}
		}
	}
	return result;
}

struct Vec_size_t floyd_warshall_reconstruct(struct ShortestPathResult *result,
                                             size_t size, size_t u, size_t v)
{
	struct Vec_size_t path = vec_size_t_new();
	if (result[u * size + v].next == INT_MAX)
		return path;
	vec_size_t_push(&path, u);
	while (u != v) {
		u = result[u * size + v].next;
		vec_size_t_push(&path, u);
	}
	return path;
}
