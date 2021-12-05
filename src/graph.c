#include "graph.h"

int edge_compare_by_weight(const void *a, const void *b)
{
	int weight_a = ((const struct Edge *)a)->weight;
	int weight_b = ((const struct Edge *)b)->weight;
	if (weight_a < weight_b)
		return -1;
	if (weight_a > weight_b)
		return 1;
	return 0;
}
