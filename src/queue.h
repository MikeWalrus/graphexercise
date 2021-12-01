#ifndef _QUEUE_H
#define _QUEUE_H

#include <stdbool.h>
#include <stddef.h>
#define QUEUE_INIT_CAPACITY 8

#define DECL_QUEUE(Type, typename)                                             \
	struct Queue_##typename                                                \
	{                                                                      \
		Type *buf;                                                     \
		size_t capacity; /* should always be power of 2 */             \
		size_t head;                                                   \
		size_t tail;                                                   \
	};                                                                     \
	size_t queue_##typename##_len(const struct Queue_##typename *queue);   \
	struct Queue_##typename queue_##typename##_new(void);                  \
	struct Queue_##typename queue_##                                       \
		typename##_with_capacity(size_t capacity);                     \
	void queue_##                                                          \
		typename##_push(struct Queue_##typename *queue, Type elem);    \
	Type queue_##typename##_pop_front(struct Queue_##typename *queue);     \
	bool queue_##                                                          \
		typename##_is_empty(const struct Queue_##typename *queue);     \
	void queue_##typename##_delete(struct Queue_##typename *queue);

DECL_QUEUE(size_t, size_t);
DECL_QUEUE(int, int);

#endif
