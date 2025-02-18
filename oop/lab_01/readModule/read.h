#ifndef READ_H
#define READ_H


#include "./errorModule/errors.h"


typedef struct edge_t
{
    int start_edge;
    int end_edge;
} edge;


typedef struct point_3d_t 
{
    double x, y, z;
} point_3d;



error_t read_amount_edges();






#endif