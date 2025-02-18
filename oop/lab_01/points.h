#ifndef POINT_H
#define POINT_H

#include <stdio.h>
#include "./errorModule/errors.h"
#include "points_operations.h"


typedef struct points
{
    point_t* array_of_points;
    int size;
} points_t;



error_t rotate_points(points_t &points, const point_t &center, const rotate_t &rotate_params);
error_t transponse_points(points_t &points, const transponse_t &transponse_params);
error_t scale_points(points_t &points, const point_t &center, const scale_t &scale_params);


#endif