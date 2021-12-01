#include <stdbool.h>

#include "src/queue.h"
#include "src/vec.h"
#include "traverse.h"

static void dfs_adj_list_graph_aux(const struct AdjListGraph *graph,
				   size_t from, size_t **trace, bool *visited);

size_t *dfs_adj_list_graph(const struct AdjListGraph *graph, size_t *ret_size)
{
	size_t size = graph->size;
	*ret_size = size;
	size_t *ret = malloc(size * sizeof(*ret));
	size_t *ret_ptr = ret;
	bool *visited = malloc(size * sizeof(*visited));
	for (size_t i = 0; i < size; i++)
		visited[i] = false;

	for (size_t i = 0; i < size; i++) {
		if (visited[i])
			continue;
		dfs_adj_list_graph_aux(graph, i, &ret_ptr, visited);
	}
	free(visited);
	return ret;
}

static void dfs_adj_list_graph_aux(const struct AdjListGraph *graph,
				   size_t from, size_t **trace, bool *visited)
{
	size_t *p = *trace;
	visited[from] = true;
	struct Vec_size_t stack = vec_size_t_new();
	vec_size_t_push(&stack, from);

	for (;;) {
		size_t curr = vec_size_t_pop_back(&stack);
		*(p++) = curr;
		struct Vec_edge *edges = &graph->adj_lists[curr];
		for (size_t i = 0; i < edges->size; i++) {
			size_t to = edges->buf[i].to;
			if (visited[to])
				continue;

			visited[to] = true;
			vec_size_t_push(&stack, to);
		}
		if (stack.size == 0)
			break;
	}
	*trace = p;
	vec_size_t_delete(&stack);
}

static void bfs_adj_list_graph_aux(const struct AdjListGraph *graph,
				   size_t from, size_t **trace, bool *visited);

size_t *bfs_adj_list_graph(const struct AdjListGraph *graph, size_t *ret_size)
{
	size_t size = graph->size;
	*ret_size = size;
	size_t *ret = malloc(size * sizeof(*ret));
	size_t *ret_ptr = ret;
	bool *visited = malloc(size * sizeof(*visited));
	for (size_t i = 0; i < size; i++)
		visited[i] = false;

	for (size_t i = 0; i < size; i++) {
		if (visited[i])
			continue;
		bfs_adj_list_graph_aux(graph, i, &ret_ptr, visited);
	}
	free(visited);
	return ret;
}

static void bfs_adj_list_graph_aux(const struct AdjListGraph *graph,
				   size_t from, size_t **trace, bool *visited)
{
	size_t *p = *trace;
	visited[from] = true;
	struct Queue_size_t queue = queue_size_t_new();
	queue_size_t_push(&queue, from);

	for (;;) {
		size_t curr = queue_size_t_pop_front(&queue);
		*(p++) = curr;
		struct Vec_edge *edges = &graph->adj_lists[curr];
		for (size_t i = 0; i < edges->size; i++) {
			size_t to = edges->buf[i].to;
			if (visited[to])
				continue;

			visited[to] = true;
			queue_size_t_push(&queue, to);
		}
		if (queue_size_t_is_empty(&queue))
			break;
	}
	*trace = p;
	queue_size_t_delete(&queue);
}
