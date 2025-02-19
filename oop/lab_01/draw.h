#ifndef DRAW_H
#define DRAW_H



#include <QGraphicsScene>
#include <QDebug>
#include "base_samples.h"
#include "points.h"
#include "edges.h"
#include "figure.h"


typedef struct 
{
    int height, width;
    QGraphicsScene *scene;
} view_t;



errors draw_figure(const figure_t &figure, view_t &view);
void draw_line(const point_t &p1, const point_t &p2, QGraphicsScene *scene);


#endif