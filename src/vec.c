#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "adjacency_list.h"
#include "graph.h"
#include "vec.h"

#define IMPL_VEC(Type, typename)                                               \
	struct Vec_##typename vec_##typename##_new(void)                       \
	{                                                                      \
		return (struct Vec_##                                          \
		        typename){ .capacity = 0, .size = 0, .buf = NULL };    \
	}                                                                      \
                                                                               \
	struct Vec_##typename vec_##typename##_with_capacity(size_t capacity)  \
	{                                                                      \
		return (struct Vec_##                                          \
		        typename){ .capacity = capacity,                       \
			           .size = 0,                                  \
			           .buf = malloc(capacity * sizeof(Type)) };   \
	}                                                                      \
                                                                               \
	void vec_##typename##_grow(struct Vec_##typename *vec)                 \
	{                                                                      \
		size_t new_capacity = vec->capacity * 2;                       \
		if (vec->capacity == 0)                                        \
			new_capacity = 2;                                      \
		vec->buf = realloc(vec->buf, new_capacity * sizeof(Type));     \
		vec->capacity = new_capacity;                                  \
	}                                                                      \
                                                                               \
	void vec_##typename##_push(struct Vec_##typename *vec, Type elem)      \
	{                                                                      \
		if (vec->size == vec->capacity)                                \
			vec_##typename##_grow(vec);                            \
		vec->buf[vec->size++] = elem;                                  \
	}                                                                      \
                                                                               \
	Type vec_##typename##_pop_back(struct Vec_##typename *vec)             \
	{                                                                      \
		return vec->buf[--vec->size];                                  \
	}                                                                      \
                                                                               \
	void vec_##typename##_delete(struct Vec_##typename *vec)               \
	{                                                                      \
		free(vec->buf);                                                \
	}                                                                      \
                                                                               \
	struct Vec_##typename vec_##                                           \
		typename##_clone(const struct Vec_##typename *vec)             \
	{                                                                      \
		struct Vec_##typename ret = *vec;                              \
		ret.buf = malloc(vec->capacity * sizeof(Type));                \
		memcpy(vec->buf, ret.buf, vec->size * sizeof(Type));           \
		return ret;                                                    \
	}

IMPL_VEC(struct AdjEdge, adj_edge);
IMPL_VEC(int, int);
IMPL_VEC(size_t, size_t);
IMPL_VEC(struct Edge, edge);
