#ifndef EDGES_H
#define EDGES_H


#include "../errorModule/errors.h"
#include <stdio.h>



typedef struct edge_t
{
    int start_edge;
    int end_edge;
} edge;



typedef struct edges
{
    edge_t *array_of_edges;
    int size;
} edges_t;


error_t read_amount_edges();
error_t read_edges();







#endif