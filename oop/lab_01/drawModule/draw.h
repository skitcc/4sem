#ifndef DRAW_H
#define DRAW_H

#include <QGraphicsScene>



typedef struct point_t
{
    double x;
    double y; 
    double z;
} point;


typedef struct line_t
{
    point_t start_line;
    point_t end_line;
} line;






#endif