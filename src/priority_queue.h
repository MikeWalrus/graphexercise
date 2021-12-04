#ifndef _PRIORITY_QUEUE_H
#define _PRIORITY_QUEUE_H

#include <stdbool.h>

#include "vec.h"

struct VertexPair {
	size_t index;
	int weight;
};

struct PriorityQueue {
	struct VertexPair *heap;
	size_t *indices;
	size_t size;
};

struct PriorityQueue priority_queue_new(size_t size);

void priority_queue_delete(struct PriorityQueue *q);

struct VertexPair priority_queue_pop_min(struct PriorityQueue *q);

/* returns false when the index is not in the heap */
bool priority_queue_decrese_key(struct PriorityQueue *q, size_t index,
                                int new_weight, bool *improved);

#endif
