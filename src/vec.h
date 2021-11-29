#ifndef _VEC_H
#define _VEC_H

#include <stddef.h>
#include <stdlib.h>

#define DECL_VEC(Type, typename)                                               \
	struct Vec_##typename                                                  \
	{                                                                      \
		size_t capacity;                                               \
		size_t size;                                                   \
		Type *buf;                                                     \
	};                                                                     \
                                                                               \
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
	void vec_##typename##_delete(struct Vec_##typename *vec)               \
	{                                                                      \
		free(vec->buf);                                                \
	}

#endif
