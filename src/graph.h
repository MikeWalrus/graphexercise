#ifndef _GRAPH_H
#define _GRAPH_H

#include <stddef.h>

struct TwoVertices {
	size_t a;
	size_t b;
};

struct Edge {
	size_t a;
	size_t b;
	int weight;
};

#endif
