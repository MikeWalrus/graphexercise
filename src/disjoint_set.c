#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "disjoint_set.h"

struct DisjointSet disjoint_set_new(size_t size)
{
	struct DisjointSet ret;
	ret.nodes = malloc(size * sizeof(*ret.nodes));
	for (size_t i = 0; i < size; i++)
		ret.nodes[i] =
			(struct DisjointSetNode){ .parent = NULL, .rank = 0 };
	ret.size = size;
	return ret;
}

struct DisjointSetNode *disjoint_set_find(struct DisjointSet *disjoint_set,
                                          size_t elem)
{
	struct DisjointSetNode *node = &disjoint_set->nodes[elem];
	struct DisjointSetNode *root = node;
	struct DisjointSetNode *parent;
	while ((parent = root->parent) != NULL) {
		root = parent;
	}
	// path compression
	while ((parent = node->parent) != NULL) {
		node->parent = root;
		node = parent;
	}
	return root;
}

bool disjoint_set_is_in_same_set(struct DisjointSet *disjoint_set, size_t a,
                                 size_t b)
{
	return disjoint_set_find(disjoint_set, a) ==
	       disjoint_set_find(disjoint_set, b);
}

void disjoint_set_union(struct DisjointSet *disjoint_set, size_t a, size_t b)
{
	struct DisjointSetNode *x = disjoint_set_find(disjoint_set, a);
	struct DisjointSetNode *y = disjoint_set_find(disjoint_set, b);
	if (x == y)
		return;

	if (x->rank > y->rank) {
		struct DisjointSetNode *tmp = x;
		x = y;
		y = tmp;
	}
	// the larger rank tree becomes the parent
	x->parent = y;

	if (x->rank == y->rank)
		y->rank++;
}

void disjoint_set_delete(struct DisjointSet *disjoint_set)
{
	free(disjoint_set->nodes);
}
