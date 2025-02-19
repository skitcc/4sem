#ifndef POINTS_OPERATIONS_H
#define POINTS_OPERATIONS_H


#include <cmath>
// #include "base_samples.h"

typedef struct
{
    double x, y, z;
} point_t;


typedef struct
{
    double dx, dy, dz;
} transponse_t;

typedef struct
{
    double kx, ky, kz;
} scale_t;

typedef struct
{
    double angle_x, angle_y, angle_z;
} rotate_t;

typedef struct 
{
    double r_cos, r_sin;
} angle_components_t;




void rotate_point(point_t &point, const point_t &center, const rotate_t &rotate_params);
void transponse_point(point_t &point, const transponse_t &transponse_params);
void scale_point(point_t &point, const point_t &center, const scale_t &scale_params);
void set_default_point(point_t &point);


#endif