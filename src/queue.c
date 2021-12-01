#include <stdlib.h>
#include <string.h>

#include "queue.h"

#define IMPL_QUEUE(Type, typename)                                             \
	static inline size_t queue_##                                          \
		typename##_get_real_index(size_t index, size_t capacity)       \
	{                                                                      \
		return index & (capacity - 1);                                 \
	}                                                                      \
                                                                               \
	size_t queue_##typename##_len(const struct Queue_##typename *queue)    \
	{                                                                      \
		return (queue->tail - queue->head) & (queue->capacity - 1);    \
	}                                                                      \
                                                                               \
	struct Queue_##typename queue_##typename##_new(void)                   \
	{                                                                      \
		struct Queue_##typename ret = (struct Queue_##typename){       \
			.buf = malloc(QUEUE_INIT_CAPACITY * sizeof(*ret.buf)), \
			.capacity = QUEUE_INIT_CAPACITY,                       \
			.head = 0,                                             \
			.tail = 0                                              \
		};                                                             \
		return ret;                                                    \
	}                                                                      \
                                                                               \
	struct Queue_##typename queue_##                                       \
		typename##_with_capacity(size_t capacity)                      \
	{                                                                      \
		struct Queue_##typename ret = (struct Queue_##typename){       \
			.buf = malloc(capacity * sizeof(*ret.buf)),            \
			.capacity = capacity,                                  \
			.head = 0,                                             \
			.tail = 0                                              \
		};                                                             \
		return ret;                                                    \
	}                                                                      \
                                                                               \
	static void queue_##typename##_grow(struct Queue_##typename *queue)    \
	{                                                                      \
		size_t old_capacity = queue->capacity;                         \
		size_t new_capacity = old_capacity * 2;                        \
		queue->buf = realloc(queue->buf,                               \
				     new_capacity * sizeof(*queue->buf));      \
                                                                               \
		/*  If [..head..tail..] then no need to copy. */               \
		/*  [..tail..head..] */                                        \
		/*  [..tail............head..] */                              \
		if (queue->head > queue->tail) {                               \
			Type *src = queue->buf + queue->head;                  \
			size_t new_head = queue->head + old_capacity;          \
			Type *dst = queue->buf + new_head;                     \
			size_t count = old_capacity - queue->head;             \
			memcpy(dst, src, sizeof(*dst) * count);                \
			queue->head = new_head;                                \
		}                                                              \
		queue->capacity = new_capacity;                                \
	}                                                                      \
                                                                               \
	void queue_##                                                          \
		typename##_push(struct Queue_##typename *queue, Type elem)     \
	{                                                                      \
		bool is_full =                                                 \
			queue_##typename##_len(queue) + 1 >= queue->capacity;  \
		if (is_full)                                                   \
			queue_##typename##_grow(queue);                        \
		queue->buf[queue->tail] = elem;                                \
		queue->tail = queue_##typename##_get_real_index(               \
			queue->tail + 1, queue->capacity);                     \
	}                                                                      \
                                                                               \
	Type queue_##typename##_pop_front(struct Queue_##typename *queue)      \
	{                                                                      \
		Type ret = queue->buf[queue->head];                            \
		queue->head = queue_##typename##_get_real_index(               \
			queue->head + 1, queue->capacity);                     \
		return ret;                                                    \
	}                                                                      \
                                                                               \
	bool queue_##typename##_is_empty(const struct Queue_##typename *queue) \
	{                                                                      \
		return queue_##typename##_len(queue) == 0;                     \
	}                                                                      \
                                                                               \
	void queue_##typename##_delete(struct Queue_##typename *queue)         \
	{                                                                      \
		free(queue->buf);                                              \
	}

IMPL_QUEUE(size_t, size_t);
IMPL_QUEUE(int, int);
