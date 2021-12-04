#include <limits.h>
#include <stdint.h>
#include <stdlib.h>

#include "priority_queue.h"
#include "src/vec.h"

#define NOT_IN_HEAP SIZE_MAX

struct PriorityQueue priority_queue_new(size_t size)
{
	struct PriorityQueue ret;
	ret.heap = malloc((size + 1) * sizeof(*ret.heap));
	ret.indices = malloc(size * sizeof(*ret.indices));
	ret.size = size;

	for (size_t i = 0; i < size; i++) {
		ret.indices[i] = i + 1;
	}

	ret.heap[0] = (struct VertexPair){ .weight = INT_MIN };

	for (size_t i = 1; i <= size; i++) {
		ret.heap[i] = (struct VertexPair){ .index = i - 1,
			                           .weight = INT_MAX };
	}

	return ret;
}

void priority_queue_delete(struct PriorityQueue *q)
{
	free(q->indices);
	free(q->heap);
}

static inline void update_internal_index(struct PriorityQueue *q, size_t old,
                                         size_t new)
{
	q->indices[q->heap[old].index] = new;
}

static inline void move_in_heap(struct PriorityQueue *q, size_t src_index,
                                size_t dst_index)
{
	update_internal_index(q, src_index, dst_index);
	q->heap[dst_index] = q->heap[src_index];
}

static void down_heap(struct PriorityQueue *q, size_t internal_index)
{
	size_t size = q->size;
	struct VertexPair *heap = q->heap;
	struct VertexPair elem = q->heap[internal_index];
	int weight = elem.weight;

	size_t i = internal_index;
	size_t j;
	int half_size = size / 2;
	while (i <= half_size) { // has children
		j = i * 2; // left child
		if (j < size /* have right child */ &&
		    heap[j].weight >
		            heap[j + 1].weight /* right child is smaller */)
			j++; // right child
		if (weight <= heap[j].weight) // shouldn't go further
			break; // now heap[i] is the place we want to place `elem` in
		move_in_heap(q, j, i); // move the child up
		i = j; // go to the proper child
	}
	q->indices[elem.index] = i;
	heap[i] = elem;
}

struct VertexPair priority_queue_pop_min(struct PriorityQueue *q)
{
	struct VertexPair min = q->heap[1];
	q->heap[1] = q->heap[q->size];
	q->size--;
	down_heap(q, 1);
	q->indices[min.index] = NOT_IN_HEAP;
	return min;
}

static void up_heap(struct PriorityQueue *q, size_t internal_index)
{
	struct VertexPair *heap = q->heap;
	struct VertexPair elem = q->heap[internal_index];
	int weight = elem.weight;
	size_t i = internal_index;

	while (weight < heap[i / 2].weight) {
		move_in_heap(q, i / 2, i);
		i = i / 2;
	}
	q->indices[elem.index] = i;
	heap[i] = elem;
}

bool priority_queue_decrese_key(struct PriorityQueue *q, size_t index,
                                int new_weight, bool *improved)
{
	if (q->indices[index] == NOT_IN_HEAP)
		return false;

	size_t internal_index = q->indices[index];
	if (new_weight < q->heap[internal_index].weight) {
		q->heap[internal_index].weight = new_weight;
		up_heap(q, internal_index);
		*improved = true;
	} else {
		*improved = false;
	}
	return true;
}
