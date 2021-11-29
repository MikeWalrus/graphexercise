#include <stddef.h>

struct Vertex {
	int value;
};

struct AdjMat {
	int *edges;
	struct Vertex *vetices;
	size_t size;
};

struct AdjMat *adj_mat_deserialise(const char *str);

char *adj_mat_serialise(struct AdjMat *graph);
