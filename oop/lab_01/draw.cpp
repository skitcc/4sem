#include "draw.h"


void convert_coord(point_t point, double width, double height)
{
    point.x += width / 2;
    point.y += height / 2;
}


void get_point()


void draw_line(const point_t &p1, const point_t &p2, QGraphicsScene *view)
{
    view->addLine(p1.x, p1.y, p2.x, p2.y);

}


void draw_lines(const points_t &points, const edges_t &edges)
{
    for (int i = 0; i < points.size; i++)
    {
        draw_line(points[i], )
    }
}