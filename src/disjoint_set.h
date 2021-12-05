#ifndef _DISJOINT_SET_H
#define _DISJOINT_SET_H

#include <stdbool.h>
#include <stddef.h>

struct DisjointSetNode {
	struct DisjointSetNode *parent;
	size_t rank;
};

struct DisjointSet {
	size_t size;
	struct DisjointSetNode *nodes;
};

struct DisjointSet disjoint_set_new(size_t size);

struct DisjointSetNode *disjoint_set_find(struct DisjointSet *disjoint_set,
                                          size_t elem);

bool disjoint_set_is_in_same_set(struct DisjointSet *disjoint_set, size_t a,
                                 size_t b);

void disjoint_set_union(struct DisjointSet *disjoint_set, size_t a, size_t b);

void disjoint_set_delete(struct DisjointSet *disjoint_set);

#endif
