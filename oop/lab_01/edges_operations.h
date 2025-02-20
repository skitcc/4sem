#ifndef EDGES_H
#define EDGES_H

#include "../errorModule/errors.h"
#include <stdio.h>

typedef struct edge_t {
    int start_edge;
    int end_edge;
} edge;

typedef struct edges {
    edge_t *array_of_edges;
    int size;
} edges_t;

errors read_all_edges(FILE *file, edges_t &edges);
void set_default_edges(edges_t &edges);
void free_edges(edges &edges);

#endif