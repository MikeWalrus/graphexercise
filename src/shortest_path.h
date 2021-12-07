#ifndef _SHORTEST_PATH_H
#define _SHORTEST_PATH_H

#include <stddef.h>

#include "adjacency_matrix.h"
#include "priority_queue.h"

struct ShortestPathResult {
	size_t next;
	int weight;
};

void dijkstra_update_adjacent_vertices(size_t curr, struct AdjMat *graph,
                                       struct PriorityQueue *q,
                                       struct ShortestPathResult *result);

struct ShortestPathResult *shortest_path_dijkstra(struct AdjMat *graph,
                                                  size_t vertex_index);

struct ShortestPathResult *shortest_path_floyd_warshall(struct AdjMat *graph);

struct Vec_size_t floyd_warshall_reconstruct(struct ShortestPathResult *result,
                                             size_t size, size_t u, size_t v);

#endif
