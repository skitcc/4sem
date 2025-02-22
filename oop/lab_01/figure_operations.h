#ifndef FIGURE_H
#define FIGURE_H

#include "errors.h"
#include "edges_operations.h"
#include "points_operations.h"
#include "single_point_operations.h"
#include <stdio.h>

typedef struct {
    points_t points;
    edges_t edges;
    point_t center;
} figure_t;

errors set_figure(figure_t &figure, const char *filename);
void free_figure(figure_t &figure);

void transponse_figure(figure_t &figure, const transponse_t &transponse_params);
void scale_figure(figure_t &figure, const scale_t &scale_params);
void rotate_figure(figure_t &figure, const rotate_t &rotate_params);

#endif