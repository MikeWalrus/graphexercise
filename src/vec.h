#ifndef _VEC_H
#define _VEC_H

#include <stddef.h>

#define DECL_VEC(Type, typename)                                               \
	struct Vec_##typename                                                  \
	{                                                                      \
		size_t capacity;                                               \
		size_t size;                                                   \
		Type *buf;                                                     \
	};                                                                     \
                                                                               \
	struct Vec_##typename vec_##typename##_new(void);                      \
                                                                               \
	struct Vec_##typename vec_##typename##_with_capacity(size_t capacity); \
                                                                               \
	void vec_##typename##_grow(struct Vec_##typename *vec);                \
                                                                               \
	void vec_##typename##_push(struct Vec_##typename *vec, Type elem);     \
                                                                               \
	Type vec_##typename##_pop_back(struct Vec_##typename *vec);            \
                                                                               \
	void vec_##typename##_delete(struct Vec_##typename *vec);              \
                                                                               \
	struct Vec_##typename vec_##                                           \
		typename##_clone(const struct Vec_##typename *vec);

struct Edge;
DECL_VEC(struct Edge, edge);
DECL_VEC(int, int);
DECL_VEC(size_t, size_t);

#endif
