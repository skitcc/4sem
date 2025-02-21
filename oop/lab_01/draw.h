#ifndef DRAW_H
#define DRAW_H

#include "figure_operations.h"
#include <QGraphicsScene>

typedef struct {
    int height, width;
    QGraphicsScene *scene;
} view_t;

typedef struct {
    point_t p1;
    point_t p2;
} edge_points_t;

errors draw_figure(const figure_t &figure, view_t *view);

#endif