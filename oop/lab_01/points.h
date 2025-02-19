#ifndef POINT_H
#define POINT_H

#include <stdio.h>
#include "./errorModule/errors.h"
#include "points_operations.h"





typedef struct points
{
    point_t* array_of_points;
    size_t size;
} points_t;



void set_default_points(points_t &points);
errors read_all_points(FILE *file, points_t &points);
void free_points(points_t &points);

errors rotate_points(points_t &points, const point_t &center, const rotate_t &rotate_params);
errors transponse_points(points_t &points, const transponse_t &transponse_params);
errors scale_points(points_t &points, const point_t &center, const scale_t &scale_params);


#endif
