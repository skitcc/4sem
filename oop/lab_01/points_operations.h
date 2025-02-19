#ifndef POINTS_OPERATIONS_H
#define POINTS_OPERATIONS_H


#include <cmath>
#include "base_samples.h"


void rotate_point(point_t &point, const point_t &center, const rotate_t &rotate_params);
void transponse_point(point_t &point, const transponse_t &transponse_params);
void scale_point(point_t &point, const point_t &center, const scale_t &scale_params);
void set_default_point(point_t &point);


#endif